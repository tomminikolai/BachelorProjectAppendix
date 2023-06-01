using ServerAPI.Models;
using Microsoft.EntityFrameworkCore;

namespace ServerAPI.Data
{
    public class DatabaseContext : DbContext
    {
        public DatabaseContext(DbContextOptions<DatabaseContext> options) : base(options)
        {
        }

        public DbSet<RobotInfoModel> RobotInfos { get; set; }

    }
}
