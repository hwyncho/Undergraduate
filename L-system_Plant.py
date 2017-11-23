"""
# 과제 03
# 학과 : 컴퓨터소프트웨어학과
# 학년 : 2학년
# 학번 : 2014726096
# 이름 : 조휘연

# Fractal Plant
"""
import turtle

def _level(n):
    rules = {"X":"F-[[X]+X]+F[+FX]-X", "F":"FF", "+":"+", "-":"-", "[":"[", "]":"]"}
    start = "X"
    string = start
    for i in range(n):
        temp = ""
        for j in string:
            temp += rules[j]
        string = temp
    return string

def _draw(s, size):
    stack = []
    for k in s:
        if k == "F":
            turtle.forward(size)
        elif k == "-":
            turtle.left(25)
        elif k == "+":
            turtle.right(25)
        elif k == "X":
            turtle.forward(0)
        elif k == "[":
            stack.append((turtle.position(), turtle.heading()))
        elif k == "]":
            turtle.penup()
            position, angle = stack.pop()
            turtle.setposition(position)
            turtle.setheading(angle)
            turtle.pendown()
        else:
            turtle.forward(0)

def _setup():
    turtle.hideturtle()
    turtle.tracer(False)
    turtle.color("#98a836")
    turtle.penup()
    x = -turtle.window_width() / 4
    y = -turtle.window_height() / 2
    turtle.goto(x, y)
    turtle.pendown()
    turtle.left(65)

_setup()
plant = _level(6)
_draw(plant, 3.3)
turtle.exitonclick()
