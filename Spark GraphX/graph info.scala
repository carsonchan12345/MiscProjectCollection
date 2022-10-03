import org.apache.spark.sql.SparkSession
// To make some of the examples work we will also need RDD
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
        println("Vertices count: " + graph.vertices.count)
        println("Edges count: " + graph.edges.count)
        println("inDegrees:")
        graph.inDegrees.sortBy(_._2, ascending=false).take(10).foreach(println)
        println("outDegrees")
        graph.outDegrees.sortBy(_._2, ascending=false).take(10).foreach(println)
    }
}