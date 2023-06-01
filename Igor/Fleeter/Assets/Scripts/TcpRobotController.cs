using System.Collections;
using System.Collections.Generic;
using System;
using System.Net.Sockets;
using System.Threading;
using UnityEditor.Experimental.GraphView;
using System.IO;
using System.Runtime.InteropServices.ComTypes;
using System.Net;
using System.Text;
using UnityEngine;

public class TcpRobotController: MonoBehaviour
{
    const int TCP_PORT = 1234;

    Socket _client;
    IPEndPoint _ipEndPoint;
    string _id = "NULL";

    public void Connect()
    {
        _client = new(
            _ipEndPoint.AddressFamily,
            SocketType.Stream,
            ProtocolType.Tcp);

        try
        {
            _client.Connect(_ipEndPoint);
        } catch(Exception e)
        {
            Debug.Log("Could not connect to a robot at IP end point: " + _ipEndPoint.ToString());
        }

    }

    private void SendString(string message)
    {
        if (_client.Connected)
        {
            var messageBytes = Encoding.UTF8.GetBytes(message);
            _client.Send(messageBytes, SocketFlags.None);
            Debug.Log($"Socket client sent message: \"{message}\"");
        }
    }

    private void Close()
    {
        if (_client == null) return;
        _client.Shutdown(SocketShutdown.Both);
        _client.Close();
    }
    private void Start()
    {
        _id = gameObject.name;

        // Get robot IP from the database server and assign the IP EndPoint
        var ipString = ApiManager.GetRobotLocalTcpIp(_id);
        Debug.Log($"{_id} at IP {ipString}");
        IPAddress ipAddress = IPAddress.Parse(ipString);
        _ipEndPoint = new IPEndPoint(ipAddress, TCP_PORT);
        
        // Connect to the robot
        Connect();
    }

    private void Update()
    {
        if (_client == null) return;
        if (!_client.Connected) return;

        // W A S or D pressed
        if (Input.GetKeyDown(KeyCode.W)) SendString("\x01");
        if (Input.GetKeyDown(KeyCode.S)) SendString("\x02");
        if (Input.GetKeyDown(KeyCode.A)) SendString("\x03");
        if (Input.GetKeyDown(KeyCode.D)) SendString("\x04");

        // Key released
        if (Input.GetKeyUp(KeyCode.W) || Input.GetKeyUp(KeyCode.A) || Input.GetKeyUp(KeyCode.S) || Input.GetKeyUp(KeyCode.D))
            SendString("\x00");

    }
}
