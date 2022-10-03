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
    print("V count is " ,g.vertices.count())#count vertices
    print("Edge count is " , g.edges.count())#count edges
    print("Indegree: ")
    g.inDegrees.orderBy('inDegree', ascending=False).show(10)
    print("outdegree: ")
    g.outDegrees.orderBy('outDegree', ascending=False).show(10)

    g1 = g.filterEdges("TIMESTAMP >= 10000 and TIMESTAMP<=50000").dropIsolatedVertices()
    print("g1 Edge count is " , g1.edges.count())
    print("g1 vertices count is " , g1.vertices.count())