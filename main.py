from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
# from pyqtgraph import PlotWidget
# import pyqtgraph as pg
# import sys
# import datetime

app = QtWidgets.QApplication([])
ui = uic.loadUi("QTdes.ui")

serial = QSerialPort()
serial.setBaudRate(115200)

portList = []
ports = QSerialPortInfo().availablePorts()
for port in ports:
    portList.append(port.portName())
ui.comL.clear()
ui.comL.addItems(portList)

def onReset():
    portlist = []
    ports = QSerialPortInfo().availablePorts()
    for port in ports:
        portlist.append(port.portName())
        ui.comL.clear()
        ui.comL.addItems(portlist)
def onOpen():
    serial.setPortName(ui.comL.currentText())
    serial.open(QIODevice.ReadWrite)
def onClose():
    serial.close()

def onRead():
    if not serial.canReadLine(): return     # выходим если нечего читать
    rx = serial.readLine()
    rxs = str(rx, 'utf-8').strip()
    data = rxs.split(',')
    if data[0] == '0':
        ui.temperaturL01.display(data[1])           #Температура
        ui.temperaturL02.display(data[2])           #Температура
def serialSend(data):
    txs = ""
    for val in data:
        txs += str(val)
        txs += ','
    txs = txs[:-1]
    txs += ';'
    serial.write(txs.encode())

def ledControl(val):
    if val == 2: val = 1;
    serialSend([0, val])
ui.ledC.stateChanged.connect(ledControl)
def Speed(val):
    serialSend([1, ui.speedSlider.value()])
ui.speedSlider.valueChanged.connect(Speed)
def Servo(val):
    serialSend([2, ui.servoSlider.value()])
ui.servoSlider.valueChanged.connect(Servo)


serial.readyRead.connect(onRead)
ui.openB.clicked.connect(onOpen)
ui.closeB.clicked.connect(onClose)
ui.resetB.clicked.connect(onReset)
ui.show()
app.exec()
