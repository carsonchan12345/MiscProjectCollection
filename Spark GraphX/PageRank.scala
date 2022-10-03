import org.apache.spark.sql.SparkSession
import org.apache.spark.rdd.RDD
import org.apache.spark.graphx.GraphLoader
import org.apache.spark.sql.functions._

object ReadTextFile {

    def main() : Unit = {
        val spark:SparkSession = SparkSession.builder()
            .master("local[1]")
            .appName("Spark SQL")
            .getOrCreate()   

        val graph = GraphLoader.edgeListFile(sc, "file:///home/ccw/hadoop-2.10.1/input/edge_list.txt")
        println("4330 top 20:")
        graph.personalizedPageRank(4330, 0.001, 0.15).vertices.sortBy(- _._2).filter(_._2 > 0).take(21).foreach(println)
        println("5730 top 20:")
        graph.personalizedPageRank(5730, 0.001, 0.15).vertices.sortBy(- _._2).filter(_._2 > 0).take(21).foreach(println)

        val ranks = graph.personalizedPageRank(5730, 0.001, 0.15).vertices.sortBy(- _._2).filter(_._2 > 0).take(2001)
        val tmp=ranks.map(x => x._1).toList //store the 2000 list or toset if you want
        //println(tmp)
        val newgraph=graph.subgraph(vpred = (id, attr) => tmp.contains(id))
        println("cii edge count is: " + newgraph.edges.count())
    }
}