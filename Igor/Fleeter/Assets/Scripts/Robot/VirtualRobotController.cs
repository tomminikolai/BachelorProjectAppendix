using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VirtualRobotController : MonoBehaviour
{
    private string _id = "NULL";

    private void Start()
    {
        _id = gameObject.name;
    }

    void Update()
    {
        if (_id != "NULL") { 
            var newPosition = ApiManager.GetRobotPosition(_id);
            transform.position = newPosition;

        }
    }
}
