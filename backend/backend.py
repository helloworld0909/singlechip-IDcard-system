import re
import serial
import database


ser = serial.Serial('COM3', 9600)
db = database.MySQL_connection()

ID = None

while True:
    data = ser.readline()
    print(data)


    if data.startswith('CARD:'):
        ID_re = re.search(r'CARD:(.*)$', data.strip())
        if ID_re is not None:
            ID = ID_re.group(1)
            print ID
            ret = db.searchUser(ID)
            print ret
            ret = tuple(map(str, ret))
            ser.write('\t'.join(ret))
            ser.write('\n')

    if data.startswith('TRAN:') and ID is not None:
        tran_re = re.search(r'TRAN:(.*)$', data.strip())
        if tran_re is not None:
            amount = float(tran_re.group(1))
            ret = db.searchUser(ID)
            if len(ret) == 2:
                userid, money = ret
                ret = db.pay(ID, amount)
                print ret
                if ret is None:
                    ser.write('Success\t\n')
                elif ret == 1:
                    ser.write('Not enough money\t\n')
        ID = None

    if data.startswith('BUS:'):
        ID_re = re.search(r'BUS:(.*)$', data.strip())
        if ID_re is not None:
            ID = ID_re.group(1)
            print ID
            ret = db.checkIn(ID)
            print ret
        ID = None
