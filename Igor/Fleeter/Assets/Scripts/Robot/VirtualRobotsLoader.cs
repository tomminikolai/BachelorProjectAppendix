using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VirtualRobotsLoader : MonoBehaviour
{
    public GameObject robot;

    string[] robotIds;

    void Start()
    {
        // Get the list of robots from API server
        robotIds = ApiManager.GetRobots();

        // Spawn virtual robots
        foreach (string robotId in robotIds)
        {
            GameObject currentRobot = Instantiate(robot);
            currentRobot.name = robotId;
            currentRobot.transform.parent = gameObject.transform;
        }
    }
}
