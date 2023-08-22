from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
from pyqtgraph import PlotWidget
import pyqtgraph as pg
import sys

import datetime




app = QtWidgets.QApplication([])
ui = uic.loadUi("QTdes.ui")

serial = QSerialPort()
serial.setBaudRate(115200)
portList = []
ports = QSerialPortInfo().availablePorts()
for port in ports:
    portList.append(port.portName())
ui.ComL.addItems(portList)

def onRead():
    if not serial.canReadLine(): return     # выходим если нечего читать
    rx = serial.readLine()
    rxs = str(rx, 'utf-8').strip()
    data = rxs.split(',')

    if data[0] == '0':
        ui.temperaturL.display(data[1])           #Температура
        if (data[1]) > str(int(32)):
             ui.TextTemp.setText("Жарко")
        elif (data[1]) > str(int(31)):
             ui.TextTemp.setText("Тепло")
        else: ui.TextTemp.setText("Холодно")

        NewLine = ""
        NewLine += '\n'
        DataLine = ""
        DataLine = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        file = open('temperature.txt', 'a')
        file.write(NewLine + (str(data[1])) + NewLine + DataLine + NewLine)
#Для точного определения температуры по времени

    if data[0] == '1':
        ui.distanceL.display(data[1])
        if (data[1]) > str(int(70)):
             ui.TextDistance.setText("Далеко")
        elif (data[1]) > str(int(30)):
             ui.TextDistance.setText("Близко")
        else: ui.TextDistance.setText("Опасно")

def onOpen():
    serial.setPortName(ui.ComL.currentText())
    serial.open(QIODevice.ReadWrite)
ui.OpenB.clicked.connect(onOpen)
def onClose():
    serial.close()
ui.CloseB.clicked.connect(onClose)
def serialSend(data):
    txs = ""
    for val in data:
        txs += str(val)
        txs += ','
    txs = txs[:-1]
    txs += ';'
    serial.write(txs.encode())

#######################################################################################################################

def ledControl(val):
    if val == 2: val = 1;
    serialSend([0, val])
ui.ledC.stateChanged.connect(ledControl)

def RGBcontrol(val):
    serialSend([1, ui.RS.value(), ui.GS.value(), ui.BS.value()])
    print(val)
ui.RS.valueChanged.connect(RGBcontrol)
ui.GS.valueChanged.connect(RGBcontrol)
ui.BS.valueChanged.connect(RGBcontrol)



def FORWARD(val):
    if val == 2: val = 1;
    serialSend([2, val])
ui.forw.clicked.connect(FORWARD)
def BACKWARD(val):
    if val == 2: val = 1;
    serialSend([3, val])
ui.back.clicked.connect(BACKWARD)
def STOP(val):
    if val == 2: val = 1;
    serialSend([4, val])
ui.stop.clicked.connect(STOP)
def LEFT(val):
    if val == 2: val = 1;
    serialSend([5, val])
ui.left.clicked.connect(LEFT)
def RIGHT(val):
    if val == 2: val = 1;
    serialSend([6, val])
ui.right.clicked.connect(RIGHT)

def MOTORspeedLEFT(val):
    serialSend([7, ui.motorSL.value()])
    print(val)
ui.motorSL.valueChanged.connect(MOTORspeedLEFT)
def MOTORspeedRIGHT(val):
    serialSend([8, ui.motorSR.value()])
    print(val)
ui.motorSR.valueChanged.connect(MOTORspeedRIGHT)



# def TimeDat():
#         timeDD = ""
#         timeDD = datetime.datetime.now().strftime('%H:%M:%S')
#         ui.timeL.setText(timeDD)
# TimeDat()

def TimeDat():
    timeDD = ""
    timeDD = datetime.datetime.now().strftime('%H:%M:%S')
ui.timeL.setText(TimeDat)













serial.readyRead.connect(onRead)
ui.show()
app.exec()
