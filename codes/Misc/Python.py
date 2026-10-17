# Decimal
from decimal import *
getcontext().prec = 6

# system setting
import sys
sys.setrecursionlimit(100000)
sys.set_int_max_str_digits(10000) # Python 3.11+

# turtle（teleport 需要 Python 3.12+）
from turtle import *

N = 3000000010
setworldcoordinates(-N, -N, N, N)
hideturtle()
speed(100)

def draw_line(a, b, c, d):
    teleport(a, b)
    goto(c, d)

def write_dot(x, y, text, diff=1): # diff = 文字的偏移
    teleport(x, y)
    dot(5, "red")

    teleport(x+N/100*diff, y+N/100*diff)
    write(text, font=("Arial", 5, "bold"))

# usage
draw_line(*a[i], *(a[i-1]))
write_dot(*a[i], str(a[i]))
# 以自己左方 100 單位為圓心向前畫一個 90 度的弧 / 正五邊形（參數可以是負的）
circle(100, extent = 90) 
circle(100, steps = 5)
left(90) # 左轉 90 度
fd(-100) # 倒退 100 單位
penup()
pendown()

tracer(0, 0) # IO 優化，必須在程式碼最後手動 update()
pos() # 回傳目前座標
heading() # 回傳目前方向： 0 是右方、90 是上方
color("red") # 改變顏色，可以用文字或是 #000000 的格式

# OOP
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, o): # use dir(int) to know operator name
        return Point(self.x+o.x, self.y+o.y)
    
    @property
    def distance(self):
        return (self.x**2 + self.y**2)**(0.5)
    
a = Point(3, 4)
print(a.distance)

# Fraction
from fractions import Fraction
a = Fraction(Decimal("1.1"))
a.numerator # 分子
a.denominator # 分母
