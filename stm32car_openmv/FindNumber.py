import time, image,sensor,math,pyb,ustruct
from image import SEARCH_EX, SEARCH_DS

from pyb import Pin, Timer
#从imgae模块引入SEARCH_EX和SEARCH_DS。使用from import仅仅引入SEARCH_EX,
#SEARCH_DS两个需要的部分，而不把image模块全部引入。

sensor.reset()

# Set sensor settings
sensor.set_contrast(1)
sensor.set_gainceiling(16)
# Max resolution for template matching with SEARCH_EX is QQVGA
sensor.set_framesize(sensor.QQVGA)
# You can set windowing to reduce the search image.

sensor.set_pixformat(sensor.GRAYSCALE)

sensor.set_windowing(0, 50, 160, 60)  #这个是观察窗口  后面ROI设置也会以这个为新的基准

# Load template.
# Template should be a small (eg. 32x32 pixels) grayscale image.
#加载模板图片
template01 = image.Image("/1.pgm")
template02 = image.Image("/2.pgm")

template1L = image.Image("/1L.pgm")
template2R = image.Image("/2R.pgm")

uart = pyb.UART(3, 115200, timeout_char = 1000)     #定义串口3变量

# 通过接收stm32F103传过来数据，来决定使用以下哪种模式
# Find_Task: 1.轮询1~2，直至识别到。2.根据f103给的值，单纯识别那个数
Find_Task = 1
# Target_Num: 目标数
Target_Num = 1
# find_flag: 标志位
find_flag = 0

x = 0
# data: 数据包·
data = [0x00,0x00,0x00,0x00,0x00,0x00]

########串口接收数据函数处理#########
def UartReceiveDate():  #函数若运行太快，会导致串口读取太快导致出错
    global Find_Task
    global Target_Num
    global x
    global data
    data[0] = uart.readchar()
    data[1] = uart.readchar()
    data[2] = uart.readchar()
    data[3] = uart.readchar()
    data[4] = uart.readchar()
    data[5] = uart.readchar()

    if data[x] == 42 and data[x+3] == 38 and x < 5:
        Find_Task =  data[x+1]
        Target_Num = data[x+2]
        Find_Task =  Find_Task - 48
        Target_Num = Target_Num - 48
        print(Find_Task, Target_Num)
        x = 0
    elif x >= 5: x = 0
    x+=1

########串口接收数据函数处理完毕#############

# 当被调用时，我们将返回timer对象
# 注意:在回调中不允许分配内存的函数  #openmv会自动把接收到的数据放在缓冲区里
'''def tick(timer):
    blue_led.toggle()

tim = Timer(2, freq=5)      # 使用定时器2创建定时器对象-以1Hz触发
tim.callback(tick) '''         # 将回调设置为tick函数

#####  FindTask == 1 时使用
#最初加载匹配
def FirstFindTemplate(template):
    R = img.find_template(template, 0.75, step=1, roi=(55, 0, 50, 40), search=SEARCH_EX)   #只检测中间的
    return R

def FirstFindedNum(R, Finded_Num):     #R: 模板匹配的对象，Finded_Num: 它所代表的数字
   global Find_Task
   global find_flag
   img.draw_rectangle(R, color=(225, 0, 0))

   #本来中值是80的，但返回值是框边缘，所以减去15就好  小于65是在左边，大于65是在右边
   LoR = 0
   find_flag = 1
   Num = Finded_Num
#   FH = bytearray([0x2C,0x12,Num, LoR, find_flag, Find_Task,0x5B])
   FH = bytearray([0x2C, Num, LoR, find_flag, Find_Task, 0x5B])
   uart.write(FH)
   print("目标车库：", Num)


######  FindTask == 2 时使用
#模板匹配
def FindTemplate(template):
    R = img.find_template(template, 0.9, step=1, roi=(0, 0, 160, 40), search=SEARCH_EX)
    return R

def FindedNum(R, Finded_Num):     #R: 参数是模板匹配的对象，Finded_Num: 所代表的数字
   global Find_Task
   global find_flag
   img.draw_rectangle(R, color=(225, 0, 0))

   #本来中值是80的，但返回值是框边缘，所以减去15就好  小于65是在左边，大于65是在右边
   if R[0] >= 65:
       LoR = 2    #2是右
   elif 0< R[0] <65:
       LoR = 1     #1是左
   find_flag = 1
   Num = Finded_Num
   FH = bytearray([0x2C,0x12,Num, LoR, find_flag, Find_Task,0x5B])
   uart.write(FH)
   print("识别到的数字是：", Num, "此数字所在方位：", LoR) # 终端打印

clock = time.clock()
# Run template matching
while (True):
    clock.tick()
    img = sensor.snapshot()
    # 串口接收数据
    UartReceiveDate()

    # find_template(template, threshold, [roi, step, search])
    # ROI: The region of interest tuple (x, y, w, h).
    # Step: The loop step used (y+=step, x+=step) use a bigger step to make it faster.
    # Search is either image.SEARCH_EX for exhaustive search or image.SEARCH_DS for diamond search
    #
    # Note1: ROI has to be smaller than the image and bigger than the template.
    # Note2: In diamond search, step and ROI are both ignored.

    # 初始识别车库数字
    if Find_Task == 1:
        #进行模板匹配
        r01 = FirstFindTemplate(template01)
        r02 = FirstFindTemplate(template02)

        #判断哪个模板匹配成功，并将成功匹配的相应数据发送给主控
        if r01:
             FirstFindedNum(r01, 1)
        elif r02:
             FirstFindedNum(r02, 2)
        '''
        else:
             FH = bytearray([0x2C,0x12,0x00, 0x00, 0x00, 0x00,0x5B])
             uart.write(FH)'''

    elif Find_Task == 2:
        #判断需要数字1, 2中断哪一个
        if Target_Num == 1:
            #进行模板匹配
            r1L = FindTemplate(template1L)
            #判断模板匹配成功，并将成功匹配的相应数据发送给主控
            if r1L:
                FindedNum(r1L, 1)
            else:
                FH = bytearray([0x2C,0x12, 0x00, 0x00, find_flag, Find_Task,0x5B])
                uart.write(FH)

        elif Target_Num == 2:
            #进行模板匹配
            r2R = FindTemplate(template2R)
            #判断哪个模板匹配成功，并将成功匹配的相应数据发送给主控

            if r2R:
                FindedNum(r2R, 2)
            else:
                FH = bytearray([0x2C,0x12, 0x00, 0x00, find_flag, Find_Task,0x5B])
                uart.write(FH)

        #else: time.sleep_ms(100)
    else: time.sleep_ms(100)

    print(clock.fps(),Find_Task, Target_Num)
