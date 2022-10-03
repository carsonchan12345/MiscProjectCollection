import org.apache.spark.sql.SparkSession
import org.apache.spark.rdd.RDD
import org.apache.spark.graphx.GraphLoader
import org.apache.spark.sql.functions._
import org.apache.spark.graphx.EdgeContext
import org.apache.spark.graphx.Graph

object PregelFunctions extends Serializable {    
    def sendMsg(ec: EdgeContext[Int,Int,Int]): Unit = {
    ec.sendToDst(ec.srcAttr+1)
}
// the vertex will contain the highest value, or distance, over all the messages
def mergeMsg(a: Int, b: Int): Int = {
math.max(a,b)
}
def propagateEdgeCount(g:Graph[Int,Int]):Graph[Int,Int] = {
val verts = g.aggregateMessages[Int](sendMsg, mergeMsg)
val g2 = Graph(verts, g.edges)
// in Tuple2[vertexId, Tuple2[old vertex data, new vertex data]]
val check = g2.vertices.join(g.vertices).map(x => x._2._1 - x._2._2).reduce(_ + _)
println(s"check: $check")
if (check > 0) // If there are no new information
propagateEdgeCount(g2)
else
g
}}


object ReadTextFile {

    def main() : Unit = {
        val spark:SparkSession = SparkSession.builder()
            .master("local[1]")
            .appName("Spark SQL")
            .getOrCreate()   

        val dag = GraphLoader.edgeListFile(sc, "file:///home/ccw/hadoop-2.10.1/input/dag_edge_list.txt")
        val initialGraph = dag.mapVertices((_,_) => 0)
        PregelFunctions.propagateEdgeCount(initialGraph).vertices.sortBy(- _._2).take(21).foreach(println)
    }
}