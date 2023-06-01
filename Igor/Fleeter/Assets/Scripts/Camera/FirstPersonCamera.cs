using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FirstPersonCamera : MonoBehaviour
{
    public Vector3 Offset = new Vector3(0, 0.1F, 0);

    public float turnSpeed = 4.0f;
    public float moveSpeed = 2.0f;
    public float minTurnAngle = -90.0f;
    public float maxTurnAngle = 90.0f; 
    private float rotX;

    public static bool Is2DMode;
    public static bool IsFollowingRobot = true;
    public GameObject RobotsGameObject;
    private List<GameObject> robots = new List<GameObject>();
    private int currentRobotId = 0;

    public bool IsCameraLocked = false;

    void Start()
    {
    }

    void Update()
    {
        if (IsCameraLocked)
            return;

        if (robots.Count == 0)
        {
            for (int id = 0; id < RobotsGameObject.transform.childCount; id++)
            {
                var currentRobot = RobotsGameObject.transform.GetChild(id);
                robots.Add(currentRobot.gameObject);
            }
        }

        if (!Is2DMode)
        {
            MouseAiming();
        }

        RobotSwitch();
        if (!IsFollowingRobot)
        {
            KeyboardMovement();
        } else
        {
            if (robots.Count > 0)
            {
                transform.position = robots[currentRobotId].transform.position + Offset;
            }
        }

    }

    void MouseAiming()
    {
        // get the mouse inputs
        float y = Input.GetAxis("Mouse X") * turnSpeed;
        rotX += Input.GetAxis("Mouse Y") * turnSpeed;
        // clamp the vertical rotation
        rotX = Mathf.Clamp(rotX, minTurnAngle, maxTurnAngle);
        // rotate the camera
        transform.eulerAngles = new Vector3(-rotX, transform.eulerAngles.y + y, 0);
    }

    void KeyboardMovement()
    {
        Vector3 dir = new Vector3(0, 0, 0);
        dir.x = Input.GetAxis("Horizontal");

        if (Is2DMode)
        {
            dir.y = Input.GetAxis("Vertical");
        } else
        {
            dir.z = Input.GetAxis("Vertical");
        }

        if (Input.GetKey(KeyCode.LeftShift))
        {
            if (Is2DMode)
            {
                dir.z = -1f;
            }
            else
            {
                dir.y = -1f;
            }

        }
        if (Input.GetKey(KeyCode.Space))
        {
            if (Is2DMode)
            {
                dir.z = 1f;
            } else
            {
                dir.y = 1f;
            }
        }

        transform.Translate(dir * moveSpeed * Time.deltaTime);
    }

    void RobotSwitch()
    {
        if (Input.GetKeyDown(KeyCode.LeftArrow))
        {
            if (currentRobotId <= 0)
            {
                currentRobotId = robots.Count-1;
            }
            currentRobotId--;
        } else if (Input.GetKeyDown(KeyCode.RightArrow))
        {
            if (currentRobotId >= robots.Count - 1)
            {
                currentRobotId = 0;
            }
            currentRobotId++;
        }
        if (Input.GetKeyDown(KeyCode.F))
        {
            IsFollowingRobot = !IsFollowingRobot;

        }
    }
}
