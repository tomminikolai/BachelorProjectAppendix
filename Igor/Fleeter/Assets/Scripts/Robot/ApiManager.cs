using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.IO;
using System.Text;
using System;
using Unity.VisualScripting;

public static class ApiManager
{
    public static Vector3 GetRobotPosition(string id)
    {
        HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://localhost:5076/GetRobotPosition/" + id);
        HttpWebResponse response = (HttpWebResponse)request.GetResponse();
        var encoding = ASCIIEncoding.ASCII;
        using (var reader = new StreamReader(response.GetResponseStream(), encoding))
        {
            var res = new Vector3();
            string responseText = reader.ReadToEnd();
            responseText = responseText.Substring(1, responseText.Length - 2).Replace(",", ";").Replace(".", ",");
            var respArr = responseText.Split(";");
            res.x = (float)double.Parse(respArr[0]);
            res.y = (float)double.Parse(respArr[1]);
            res.z = (float)double.Parse(respArr[2]);
            return res;
        }

    }
    
    public static string[] GetRobots()
    {
        HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://localhost:5076/GetRobots");
        HttpWebResponse response = (HttpWebResponse)request.GetResponse();
        var encoding = ASCIIEncoding.ASCII;
        using (var reader = new StreamReader(response.GetResponseStream(), encoding))
        {
            string responseText = reader.ReadToEnd();
            responseText = responseText.Substring(1, responseText.Length - 2).Replace("\"", "");
            var respArr = responseText.Split(",");
            return respArr;
        }
    }
    public static string GetRobotLocalTcpIp(string id)
    {
        HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://localhost:5076/GetLocalTcpIp/" + id);
        HttpWebResponse response = (HttpWebResponse)request.GetResponse();
        var encoding = ASCIIEncoding.ASCII;
        using (var reader = new StreamReader(response.GetResponseStream(), encoding))
        {
            string responseText = reader.ReadToEnd();
            return responseText;
        }
    }
}
