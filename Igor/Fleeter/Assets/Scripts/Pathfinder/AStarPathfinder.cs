using System;
using System.Collections.Generic;
using System.Threading.Tasks;



public class AStarPathfinder
{
    private bool[,] _map;
    private int _width;
    private int _height;
    private Node[,] _nodes;

    public AStarPathfinder(bool[,] map)
    {
        _map = map;
        _width = map.GetLength(0);
        _height = map.GetLength(1);
        _nodes = new Node[_width, _height];
        for (int x = 0; x < _width; x++)
        {
            for (int y = 0; y < _height; y++)
            {
                _nodes[x, y] = new Node { X = x, Y = y };
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

    private int Heuristic(Node node, Node target)
    {
        return Math.Abs(node.X - target.X) + Math.Abs(node.Y - target.Y);
    }

    public List<Node> FindShortestPath(int startX, int startY, int endX, int endY)
    {
        var startNode = _nodes[startX, startY];
        var targetNode = _nodes[endX, endY];

        var openSet = new HashSet<Node>();
        var closedSet = new HashSet<Node>();
        openSet.Add(startNode);

        while (openSet.Count > 0)
        {
            var currentNode = openSet.MinBy(node => node.F);
            if (currentNode == targetNode) break;
            openSet.Remove(currentNode);
            closedSet.Add(currentNode);
            foreach (var neighbor in GetNeighbors(currentNode))
            {
                if (closedSet.Contains(neighbor)) continue;
                var tentativeGScore = currentNode.G + 1;
                if (!openSet.Contains(neighbor) || tentativeGScore < neighbor.G)
                {
                    neighbor.G = tentativeGScore;
                    neighbor.H = Heuristic(neighbor, targetNode);
                    neighbor.Parent = currentNode;
                    if (!openSet.Contains(neighbor))
                    {
                        openSet.Add(neighbor);
                    }
                }
            }
        }

        var path = new List<Node>();
        var current = targetNode;
        while (current != startNode)
        {
            path.Add(current);
            current = current.Parent;
        }
        path.Add(startNode);
        path.Reverse();
        return path;
    }
}
