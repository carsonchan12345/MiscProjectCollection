import os
from pyspark import SparkContext
from pyspark.sql import SQLContext
from pyspark.sql import SparkSession
from graphframes import *
if __name__ == "__main__":
    spark = SparkSession \
        .builder \
        .appName("Spark SQL") \
        .getOrCreate()
    spark.sparkContext.setLogLevel("ERROR")
    e = spark.read.csv("file:///home/ccw/hadoop-2.10.1/input/mooc_actions.tsv", sep=r'\t', header=True, inferSchema='true')
    v = spark.read.csv("file:///home/ccw/hadoop-2.10.1/input/vertices.tsv", sep=r'\t', header=True, inferSchema='true')
    g = GraphFrame(v, e)
    g1 = g.filterEdges("TIMESTAMP >= 10000 and TIMESTAMP<=50000").dropIsolatedVertices()
    motifs1 = g1.find("(a)-[e1]->(b); (c)-[e2]->(b)")
    motifs1 =motifs1.filter("a.id != c.id")
    motifs1 =motifs1.filter("e1.TIMESTAMP <= e2.TIMESTAMP")
    print("i count= ", motifs1.count())
    motifs2 = g1.find("(a)-[ab]->(b); (b)-[bc]->(c)")
    print("ii count= ", motifs2.count())
    motifs2 =motifs2.filter("a.id != c.id")
    print("ii count= ", motifs2.count())
    motifs3 = g1.find("(a)-[e1]->(d); (a)-[e2]->(c); (b)-[e3]->(c); (b)-[e4]->(d)")
    motifs3 =motifs3.filter("a.id != b.id")
    motifs3 =motifs3.filter("c.id != d.id")
    motifs3 =motifs3.filter("e1.TIMESTAMP <= e4.TIMESTAMP and e1.TIMESTAMP <= e3.TIMESTAMP")
    print("iii count= ", motifs3.count())
    motifs4 = g1.find("(a)-[e1]->(c); (a)-[e2]->(d); (b)-[e3]->(d); (b)-[e4]->(f)")
    motifs4 =motifs4.filter("a.id != b.id")
    motifs4 =motifs4.filter("c.id != f.id")
    motifs4 =motifs4.filter("e2.TIMESTAMP <= e3.TIMESTAMP and e1.TIMESTAMP <= e4.TIMESTAMP")
    print("iv count= ", motifs4.count())