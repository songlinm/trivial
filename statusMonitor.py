import urllib.request
import ssl
import time
import threading
from html.parser import HTMLParser
import os
from enum import Enum
import smtplib
import configparser

class ParseState(Enum):
    Normal = 1
    FoundJubileeSpan = 2
    FoundJubileeDisruptionSummarySpan = 3
    ReadyToReadData = 4
    Finished = 5

class ServiceStatus:
    def __init__(self):
        self.statusStr = ''
        self.timestamp = ''

class ServiceStatusParser(HTMLParser):
    def __init__(self, serviceStatus):
        super().__init__()
        self.state = ParseState.Normal
        self.isInSpan = False
        self.isInSpan = False
        self.serviceStatus = serviceStatus

    def handle_starttag(self, tag, attrs):
        if tag == 'span':
            self.isInSpan = True;
            if self.state == ParseState.FoundJubileeSpan:
                for attr in attrs:
                    if attr[0] == 'class' and 'disruption-summary' in attr[1]:
                        self.state = ParseState.FoundJubileeDisruptionSummarySpan
            elif self.state == ParseState.FoundJubileeDisruptionSummarySpan:
                self.state = ParseState.ReadyToReadData
        else:
            self.isInSpan = False

    def handle_data(self, data):
        if self.isInSpan:
            if data == 'Jubilee':
                self.state = ParseState.FoundJubileeSpan
            elif self.state == ParseState.ReadyToReadData:
                self.serviceStatus.statusStr = data.rstrip().lstrip()
                self.serviceStatus.timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
                self.state = ParseState.Finished

def monitor():
    ctx = ssl.create_default_context()
    ctx.check_hostname = False
    ctx.verify_mode = ssl.CERT_NONE

    oldStatus = ServiceStatus()
    exitFlag = threading.Event()
    opener = urllib.request.build_opener(urllib.request.HTTPSHandler(context=ctx))
    opener.addheaders = [('User-Agent', 
                          'Mozilla/5.0 (Windows NT 6.3; Trident/7.0; rv:11.0) like Gecko')]
    while True:
        newStatus = ServiceStatus()

        try:
            with opener.open('https://tfl.gov.uk/tube-dlr-overground/status/') as f:
                if f.getcode() == 200:
                    parser = ServiceStatusParser(newStatus)
                    parser.feed(f.read().decode('utf-8'))
                    handleServiceStatus(oldStatus, newStatus)
                    oldStatus = newStatus
        except Exception as inst:
            print (inst)

        if exitFlag.wait(timeout=600):
            return

def handleServiceStatus(oldStatus, newStatus):
    print (newStatus.timestamp, ': ', newStatus.statusStr)
    print ('  old status - ', oldStatus.timestamp, ': ', oldStatus.statusStr)
    if ('good' not in newStatus.statusStr.lower() and 
        'closed' not in newStatus.statusStr.lower()):
        if ('good' in oldStatus.statusStr.lower() or 
            'closed' in oldStatus.statusStr.lower()):
            EmailSender().send('Jubilee line service alert', 
                               'Service status ' + newStatus.statusStr)

class EmailSender:
    def __init__(self):
        config = configparser.ConfigParser()
        config.read([os.path.expanduser('~/.tflstatus.cfg')])
        self.user = config['DEFAULT']['User']
        self.passwd = config['DEFAULT']['Password']
        self.recipient = self.user
        if not self.user or not self.passwd:
            print("Email will not be sent due to missing User or Password")

    def send(self, subject, body):
        if not self.user or not self.passwd:
            return
        FROM = self.user
        TO = self.recipient if type(self.recipient) is list else [self.recipient]
        SUBJECT = subject
        TEXT = body

        # Prepare actual message
        message = """From: %s\nTo: %s\nSubject: %s\n\n%s
        """ % (FROM, ", ".join(TO), SUBJECT, TEXT)
        try:
            server = smtplib.SMTP("smtp.gmail.com", 587)
            server.ehlo()
            server.starttls()
            server.login(self.user, self.passwd)
            server.sendmail(FROM, TO, message)
            server.close()
            print('successfully sent the mail to ', self.recipient)
        except:
            print("failed to send mail to ", self.recipient)

if __name__ == "__main__":
    monitor()
