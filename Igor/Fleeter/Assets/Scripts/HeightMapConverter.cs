using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class HeightMapConverter : MonoBehaviour
{

    public bool[,] grid;

    private Gradient gradient;
    private GradientColorKey[] colorKey;
    private GradientAlphaKey[] alphaKey;

    private bool firstIteration = true;


    void TestPathfinding()
    {
        Debug.Log("Testing pathfinding.");
        var astar = new AStarPathfinder(grid);

        string path = "Assets/Resources/nodelist.txt";
        //Write some text to the test.txt file
        StreamWriter writer = new StreamWriter(path, true);

        for (int k = 0; k < grid.GetLength(0); k++)
        {
            string x = "{";
            for (int l = 0; l < grid.GetLength(1); l++)
            {
                x += grid[k, l].ToString() + ", ";
            }
            writer.WriteLine(x.Substring(0, x.Length - 2) + "},");
        }
        writer.Close();

    }

    void DrawPath(List<Node> path)
    {
        var image = GetComponent<Image>();

        Texture2D texture = image.sprite.texture;

        Texture2D newTexture = new Texture2D(texture.width, texture.height);
        newTexture.SetPixels(texture.GetPixels());
        newTexture.Apply();

        foreach (var node in path)
        {
            newTexture.SetPixel(node.X, node.Y, Color.blue);
        }
        newTexture.Apply();

        image.sprite = Sprite.Create(newTexture, new Rect(0, 0, newTexture.width, newTexture.height), new Vector2(0.5f, 0.5f));
    }

    void InitializeHeightmapAndGrid()
    {

        gradient = new Gradient();

        // Populate the color keys at the relative time 0 and 1 (0 and 100%)
        colorKey = new GradientColorKey[2];
        colorKey[0].color = Color.green;
        colorKey[0].time = 0.0f;
        colorKey[1].color = Color.red;
        colorKey[1].time = 1.0f;

        // Populate the alpha  keys at relative time 0 and 1  (0 and 100%)
        alphaKey = new GradientAlphaKey[2];
        alphaKey[0].alpha = 1.0f;
        alphaKey[0].time = 0.0f;
        alphaKey[1].alpha = 0.0f;
        alphaKey[1].time = 1.0f;

        gradient.SetKeys(colorKey, alphaKey);

        var image = GetComponent<Image>();
        //image.sprite.texture.GetPixel(0, 0);

        Texture2D texture = image.sprite.texture;
        Texture2D newTexture = new Texture2D(texture.width, texture.height);

        grid = new bool[texture.width, texture.height];

        for (int x = 0; x < texture.width; x++)
        {
            for (int y = 0; y < texture.height; y++)
            {
                float height = texture.GetPixel(x, y).grayscale;
                Color32 color;
                if (height < 0.1)
                {
                    color = Color.green;
                    grid[x, y] = true;
                }
                else
                {
                    color = Color.red;
                    grid[x, y] = false;
                }
                newTexture.SetPixel(x, y, color);
            }
        }

        Debug.Log("Finished coloring the height map.");

        newTexture.Apply();

        image.sprite = Sprite.Create(newTexture, new Rect(0, 0, newTexture.width, newTexture.height), new Vector2(0.5f, 0.5f));
    }

    // Start is called before the first frame update
    void Start()
    {
        InitializeHeightmapAndGrid();

    }

    // Update is called once per frame
    void Update()
    {
        if (firstIteration)
        {
            TestPathfinding();
            firstIteration = false;
        }
    }

}
