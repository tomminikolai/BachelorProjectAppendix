# Waypoints pathfinding
This repository allows a user to pathfind using various algorithms using a graph data structure (called WaypointMap in the application, a node is called WaypointNode).

Currently supports only Dijkstra algorithm (only relative posiitons, no absolute ones necessary for A*).

Example use:
```C#
using WaypointPathfinding;

// Initialize a WaypointMap
WaypointMap map = new WaypointMap();

// Add empty, arbitrary points
for (int i = 0; i < 5; i++)
{
    
    WaypointNode node = new WaypointNode(i);
    map.AddWaypoint(node);
}

// Add arbitrary connections with distances
map.AddConnectionBetweenTwoNodes(map.Waypoints[0], map.Waypoints[1], 6);
map.AddConnectionBetweenTwoNodes(map.Waypoints[0], map.Waypoints[3], 1);
map.AddConnectionBetweenTwoNodes(map.Waypoints[1], map.Waypoints[3], 2);
map.AddConnectionBetweenTwoNodes(map.Waypoints[1], map.Waypoints[4], 2);
map.AddConnectionBetweenTwoNodes(map.Waypoints[1], map.Waypoints[2], 5);
map.AddConnectionBetweenTwoNodes(map.Waypoints[2], map.Waypoints[1], 5);
map.AddConnectionBetweenTwoNodes(map.Waypoints[2], map.Waypoints[4], 5);
map.AddConnectionBetweenTwoNodes(map.Waypoints[3], map.Waypoints[0], 1);
map.AddConnectionBetweenTwoNodes(map.Waypoints[3], map.Waypoints[1], 2);
map.AddConnectionBetweenTwoNodes(map.Waypoints[3], map.Waypoints[4], 1);
map.AddConnectionBetweenTwoNodes(map.Waypoints[4], map.Waypoints[3], 1);
map.AddConnectionBetweenTwoNodes(map.Waypoints[4], map.Waypoints[1], 2);
map.AddConnectionBetweenTwoNodes(map.Waypoints[4], map.Waypoints[2], 5);

// Initialize Dijkstra pathfinder
DijkstraPathfinder pathfinder = new DijkstraPathfinder(map);

// Obtain path a path. Returns a list of WaypointNode
var path = pathfinder.GetPath(map.Waypoints[0], map.Waypoints[2]);

// Iterate through path and print node IDs
foreach(var node in path)
{
    Console.WriteLine(node.Id);
}
```

The WaypointMapParser class contains useful formats to parse different formats into WaypointMap. 
Currently only supports parsing from a list of strings. Usage:
```
using Pathfinding;

var mapFromArray = WaypointMapParser.ParseArrayOfStrings(new List<string> { 
    "1;[2,5];[5.1,2.2]",
    "2;[1];[5.1]",
    "5;[1];[2.2]"
});

foreach (WaypointNode node in mapFromArray.Waypoints)
{
    Console.WriteLine("CONNECTIONS FOR" + node.Id.ToString());
    foreach (var kvp in node.ConnectionsAndDistances)
    {
        Console.WriteLine("Conn id=" + kvp.Key.Id + "has distance " + kvp.Value);
    }
}
```