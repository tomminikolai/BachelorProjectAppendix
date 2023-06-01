using System;
using System.Collections.Generic;
public static class Extensions
{
    public static T MinBy<T, TKey>(this IEnumerable<T> source, Func<T, TKey> selector) where TKey : IComparable<TKey>
    {
        using var enumerator = source.GetEnumerator();
        if (!enumerator.MoveNext())
            throw new InvalidOperationException("Sequence contains no elements");

        T min = enumerator.Current;
        TKey minKey = selector(min);

        while (enumerator.MoveNext())
        {
            var current = enumerator.Current;
            var currentKey = selector(current);

            if (currentKey.CompareTo(minKey) < 0)
            {
                min = current;
                minKey = currentKey;
            }
        }

        return min;
    }
}

public class Node
{
    public int X { get; set; }
    public int Y { get; set; }
    public int G { get; set; } // cost from start node
    public int H { get; set; } // heuristic cost to target node
    public int F { get { return G + H; } } // total cost
    public Node Parent { get; set; }
    public int Distance { get; set; }
}