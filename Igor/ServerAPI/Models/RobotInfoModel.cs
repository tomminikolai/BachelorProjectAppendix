using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ServerAPI.Models
{
    public class RobotInfoModel
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }
        public string Name { get; set; } = string.Empty;
        public double PositionX { get; set; } = 0;
        public double PositionY { get; set; } = 0;
        public double PositionZ { get; set; } = 0;
        public double Rotation { get; set; } = 0;
        public bool Busy { get; set; } = false;
        public string LocalTcpIp { get; set; } = string.Empty;
    }
}