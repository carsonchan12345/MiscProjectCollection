import os
import random
import time
from datetime import datetime
import subprocess
from subprocess import PIPE, Popen
# modify to convert the ts to seconds  
def convert_to_seconds(ts, last_ts): 
    tmp=0
    if last_ts is not None:
        datetime_object1 = datetime.strptime(last_ts, '%Y-%m-%d %H:%M:%S')
        datetime_object2 = datetime.strptime(ts, '%Y-%m-%d %H:%M:%S')
        tmp=((datetime_object2-datetime_object1).total_seconds())/60

    return tmp

# modify this function, the sleep time should based on the time in the data
def random_sleep(ts):
 # modify this line
    time.sleep(ts)

def main():
    p = subprocess.Popen('./bin/kafka-console-producer.sh --broker-list kafka-headless:9092 --topic IERG4330', shell=True, stdin=PIPE, stdout=PIPE)

    last_ts = None
    with open('new_tweets.txt',"r",encoding="utf-8") as f:
        for line in f:

            # split the text and timestamp
            parts = line.rstrip().split(',')
            text = ' '.join(parts[:-1])
            ts = parts[-1]

            tmpts = convert_to_seconds(ts,last_ts)     

            if last_ts is not None:
                random_sleep(tmpts)

            #cmd = 'echo "' + text + '" | ./bin/kafka-console-producer.sh --broker-list kafka-headless:9092 --topic IERG4330'
            #os.system(cmd)
            tmp=text+" \n"
            p.stdin.write(bytes(tmp, 'utf-8'))
            print(tmp)
            last_ts = ts


if __name__ == '__main__':
    main()
