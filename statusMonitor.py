import urllib.request
import ssl
import time
import threading
from html.parser import HTMLParser
from enum import Enum
import smtplib

class ParseState(Enum):
    Normal = 1
    FoundJubileeSpan = 2
    FoundJubileeDisruptionSummarySpan = 3
    ReadyToReadData = 4
    Finished = 5

class ServiceStatusParser(HTMLParser):
    def __init__(self):
        super().__init__()
        self.state = ParseState.Normal
        self.isInSpan = False

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
                data = data.rstrip().lstrip()
                if 'Good service' not in data:
                    subject = 'Service is *NOT* good'
                    body = data
                    EmailSender().send(subject, body)
                else:
                    subject = 'Service is Good'
                    body = data
                print(time.strftime("%Y-%m-%d %H:%M:%S"), ': Service is ', data)
                self.state = ParseState.Finished

def monitor():
    ctx = ssl.create_default_context()
    ctx.check_hostname = False
    ctx.verify_mode = ssl.CERT_NONE

    exitFlag = threading.Event()
    while True:
        with urllib.request.urlopen('https://tfl.gov.uk/tube-dlr-overground/status/', 
                                    context=ctx) as f:
            parser = ServiceStatusParser()
            parser.feed(f.read().decode('utf-8'))
        if exitFlag.wait(timeout=20):
            return


class EmailSender:
    def __init__(self):
        self.user = ToRead
        self.passwd = ToRead 
        self.recipient = ToRead

    def send(self, subject, body):
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
