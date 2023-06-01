using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using ServerAPI.Data;
using ServerAPI.Models;
using System.Diagnostics;

namespace ServerAPI.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class RobotDataController : ControllerBase
    {

        private DatabaseContext _db;

        public RobotDataController(ILogger<RobotDataController> logger, DatabaseContext db)
        {
            _db = db;
        }

        [HttpGet("/GetRobotPosition/{id}")]
        public IEnumerable<double> GetRobotPosition(string id)
        {

            var robotInfo = _db.RobotInfos.Where(x => x.Id.ToString() == id).ToList()[0];

            return new double[] { robotInfo.PositionX, robotInfo.PositionY, robotInfo.PositionZ };
        }

        [HttpGet("/GetRobotRotation/{id}")]
        public double GetRobotRotation(string id)
        {

            var robotInfo = _db.RobotInfos.Where(x => x.Id.ToString() == id).ToList()[0];

            return robotInfo.Rotation;
        }

        [HttpGet("/GetRobotPositionAndRotation/{id}")]
        public IEnumerable<double> GetRobotPositionAndRotation(string id)
        {
            var robotInfo = _db.RobotInfos.Where(x => x.Id.ToString() == id).ToList()[0];

            return new double[] { robotInfo.PositionX, robotInfo.PositionY, robotInfo.PositionZ, robotInfo.Rotation };
        }

        [HttpGet("/GetLocalTCPIP/{id}")]
        public String GetRobotTcpIp(string id)
        {
            var robotInfo = _db.RobotInfos.Where(x => x.Id.ToString() == id).ToList().FirstOrDefault();

            return robotInfo?.LocalTcpIp ?? "NULL";

        }

        [HttpGet("/GetRobots")]
        public IEnumerable<string> GetRobots()
        {
            var ids = new List<string>();

            foreach (var robotInfo in _db.RobotInfos)
            {
                ids.Add(robotInfo.Id.ToString());
            }
            return ids.ToArray();
        }

        [HttpGet("/SetRobotPosition/{id}/position={x};{y};{z}")]
        public void SetRobotPosition(string id, string x, string y, string z)
        {
            var robotInfo = _db.RobotInfos.Where(x => x.Id.ToString() == id).ToList()[0];

            if (robotInfo != null)
            {
                robotInfo.PositionX = float.Parse(x.Replace(",", "."));
                robotInfo.PositionY = float.Parse(y.Replace(",", "."));
                robotInfo.PositionZ = float.Parse(z.Replace(",", "."));

                _db.SaveChanges();
            }
        }

        [HttpGet("/SetRobotRotation/{id}/rotation={rotation}")]
        public void SetRobotRotation(string id, string rotation)
        {
            var robotInfo = _db.RobotInfos.Where(x => x.Id.ToString() == id).ToList()[0];

            if (robotInfo != null)
            {
                robotInfo.Rotation = float.Parse(rotation.Replace(",", "."));

                _db.SaveChanges();
            }
        }

        [HttpGet("/SetRobotPositionAndRotation/{id}/position={x};{y};{z}&rotation={rotation}")]
        public void SetRobotPositionAndRotation(string id, string x, string y, string z, string rotation)
        {
            var robotInfo = _db.RobotInfos.Where(x => x.Id.ToString() == id).ToList().FirstOrDefault();

            if (robotInfo != null)
            {
                robotInfo.PositionX = float.Parse(x.Replace(",", "."));
                robotInfo.PositionY = float.Parse(y.Replace(",", "."));
                robotInfo.PositionZ = float.Parse(z.Replace(",", "."));
                robotInfo.Rotation = float.Parse(rotation.Replace(",", "."));

                _db.SaveChanges();
            }
        }

        [HttpGet("/SetLocalTCPIP/{id}/ip={tcpIp}")]
        public void SetRobotTcpIp(string id, string tcpIp)
        {
            var robotInfo = _db.RobotInfos.Where(x => x.Id.ToString() == id).ToList().FirstOrDefault();

            if (robotInfo != null)
            {
                robotInfo.LocalTcpIp = tcpIp;

                _db.SaveChanges();
            }
        }

        // Returns ID of added robot
        [HttpGet("/AddRobot/name={name}")]
        public string AddRobot(string name)
        {
            var newRobotInfo = new RobotInfoModel();
            newRobotInfo.Name = name;

            _db.RobotInfos.Add(newRobotInfo);

            _db.SaveChanges();

            int id = newRobotInfo.Id;

            return id.ToString();
        }
    }

}