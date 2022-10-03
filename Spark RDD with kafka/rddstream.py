import os
from pyspark import SparkContext
from pyspark.streaming import StreamingContext
from pyspark.streaming.kafka import KafkaUtils
import json





def main():
    sc = SparkContext(appName="PythonSparkStreamingKafka")
    sc.setLogLevel("WARN")
    ssc = StreamingContext(sc, 1)
    ssc.checkpoint("hdfs:///x.x.x.x/xxxxx")
    #lines = ssc.socketTextStream("localhost", xxxx) for local test
    lines = KafkaUtils.createStream(ssc, 'kafka-zookeeper:2181', 'spark-streaming', {'IERG4330':1})
    words = lines.flatMap(lambda line: line.split(" "))
    hashtags = words.filter(lambda w: '#' in w).map(lambda x: (x, 1))
    #hashtagsCounts = hashtags.reduceByKey(lambda x, y: x + y)
    windowedline = hashtags.reduceByKeyAndWindow(lambda x, y: x + y, lambda x, y: x - y, 5, 2).transform(lambda rdd:rdd.sortBy(lambda a:a[1], ascending=False))
    topwindowedline=windowedline.pprint(30)
    ssc.start()
    ssc.awaitTermination()
    #lines = ssc.socketTextStream("localhost", 9999)
    #words = lines.flatMap(lambda line: line.split(" "))
    #hashtags = words.filter(lambda w: '#' in w).map(lambda x: (x, 1))
if __name__ == '__main__':
    main()