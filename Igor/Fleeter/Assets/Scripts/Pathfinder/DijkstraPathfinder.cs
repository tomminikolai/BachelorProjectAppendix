using System;
using System.Collections.Generic;
using System.Threading.Tasks;


public class DijkstraPathfinder
{
    private bool[,] _map;
    private int _width;
    private int _height;
    private Node[,] _nodes;
    public DijkstraPathfinder(bool[,] map)
    {
        _map = map;
        _width = map.GetLength(0);
        _height = map.GetLength(1);
        _nodes = new Node[_width, _height];
        for (int x = 0; x < _width; x++)
        {
            for (int y = 0; y < _height; y++)
            {
                _nodes[x, y] = new Node { X = x, Y = y, Distance = int.MaxValue };
            }
        }
    }

    private List<Node> GetNeighbors(Node node)
    {
        var neighbors = new List<Node>();
        if (node.X > 0 && _map[node.X - 1, node.Y]) neighbors.Add(_nodes[node.X - 1, node.Y]);
        if (node.X < _width - 1 && _map[node.X + 1, node.Y]) neighbors.Add(_nodes[node.X + 1, node.Y]);
        if (node.Y > 0 && _map[node.X, node.Y - 1]) neighbors.Add(_nodes[node.X, node.Y - 1]);
        if (node.Y < _height - 1 && _map[node.X, node.Y + 1]) neighbors.Add(_nodes[node.X, node.Y + 1]);
        return neighbors;
    }

    public List<Node> FindShortestPath(int startX, int startY, int endX, int endY)
    {
        _nodes[startX, startY].Distance = 0;
        var unvisitedNodes = new HashSet<Node>();
        for (int x = 0; x < _width; x++)
        {
            for (int y = 0; y < _height; y++)
            {
                unvisitedNodes.Add(_nodes[x, y]);
            }
        }
        while (unvisitedNodes.Count > 0)
        {
            var currentNode = unvisitedNodes.MinBy(n => n.Distance);
            if (currentNode.Distance == int.MaxValue) break;
            unvisitedNodes.Remove(currentNode);
            foreach (var neighbor in GetNeighbors(currentNode))
            {
                var tentativeDistance = currentNode.Distance + 1;
                if (tentativeDistance < neighbor.Distance)
                {
                    neighbor.Distance = tentativeDistance;
                }
            }
        }
        var path = new List<Node>();
        var current = _nodes[endX, endY];
        while (current != _nodes[startX, startY])
        {
            path.Add(current);
            current = GetNeighbors(current).MinBy(n => n.Distance);
        }
        path.Add(_nodes[startX, startY]);
        path.Reverse();

        return path;
    }

}
