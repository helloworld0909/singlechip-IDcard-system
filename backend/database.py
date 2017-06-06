import MySQLdb
import settings
import time


class MySQL_connection(object):

    def __init__(self):
        self.connection = MySQLdb.connect(
            host=settings.MYSQL_HOST,
            port=settings.MYSQL_PORT,
            user=settings.MYSQL_USER,
            passwd=settings.MYSQL_PASSWD,
            db=settings.MYSQL_DB,
            charset=settings.MYSQL_CHARSET
        )
        self.cursor = self.connection.cursor()
        self.cursor.execute('use payment;')

    def searchUser(self, ID):
        self.cursor.execute('select id, money from balance where cardid="{}";'.format(ID))
        ret = self.cursor.fetchone()
        if ret:
            return ret
        else:
            return []

    def pay(self, ID, amount):
        try:
            self.cursor.execute('update balance set money=money-{} where cardid="{}";'.format(amount, ID))
            self.connection.commit()
            return self.cursor.fetchone()
        except:
            self.connection.rollback
            return 1

    def searchUserInfo(self, ID):
        self.cursor.execute('select id, username from balance where cardid="{}";'.format(ID))
        ret = self.cursor.fetchone()
        if ret:
            return ret
        else:
            return []

    def checkIn(self, ID):
        try:
            userid, username = self.searchUserInfo(ID)
            sql = 'insert into buscheckin values("{}", "{}", "{}", "{}");'.format(userid, ID, username, time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time())))
            print sql
            self.cursor.execute(sql)
            self.connection.commit()
        except:
            self.connection.rollback
            return 1
