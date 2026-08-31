#include "WeaponSystem.h"
//#include "raylib.h"
#include "raymath.h"
#include "Player.h"
#include "ActivePowerup.h"

void WeaponSystem::Update(GameWorld& world, Player& player,const EnemyDatabase& enemyDatabase, const WeaponDatabase& weaponDatabase, Vector2 playerPos, float deltaTime)
{   


    //-----------------Enemy Weapon Logic-----------------
    for (size_t i = 0; i < world.enemies.size(); ++i)
    {
        Enemy& enemy = world.enemies[i];

        if (enemy.state != EnemyState::Attacking)
        {
            continue; // Skip enemies that are not in the Attacking state
        }

        const EnemyDefinition& definition = enemyDatabase.Get(enemy.type);
        const WeaponDefinition& weaponDef = weaponDatabase.Get(definition.weaponType);

        enemy.weaponCooldown -= deltaTime;
        if (enemy.weaponCooldown > 0.f)
        {
            continue; // Skip if the weapon is still on cooldown
        }

        const Vector2 enemyPosition = world.enemyPositions[i].value;
        Vector2 dir = Vector2Subtract(playerPos, enemyPosition);
        dir = Vector2Normalize(dir);

        //here wego through the number of projectiles defined in the weapon definition and spawn them accordingly
        for (uint8_t shot = 0; shot < weaponDef.projectileCount; ++shot)
        {
            Vector2 projectileDirection = dir;
            if (weaponDef.projectileCount > 1)
            {
                const float spreadAngle = weaponDef.projectileSpread;
                //offset the direction of the projectile based on its index and the spread angle
                const float offset = (shot == 0) ? -spreadAngle : spreadAngle;
                // Rotate the projectile direction by the offset angle
                projectileDirection = Vector2Rotate(projectileDirection, offset * DEG2RAD);
            }

            Projectile projectile{};
            projectile.owner = ProjectileOwner::Enemy;
            projectile.damage = weaponDef.damage;
            projectile.lifetime = 5.f;
            world.projectiles.push_back(projectile);
            world.projectilePositions.push_back(Position{ enemyPosition });
            world.projectileVelocities.push_back(Velocity{ Vector2Scale(projectileDirection, weaponDef.projectileSpeed) });
        }

        // Reset the weapon cooldown based on the weapon's fire rate
        enemy.weaponCooldown = 1.0f / weaponDef.fireRate;

    }
        //-----------------Player Weapon-----------------
        if (!player.IsShootPressed())
        {
            return; // Player is not trying to shoot, exit the function
        }

        const WeaponDefinition& wepdef = weaponDatabase.Get(WeaponType::Single);
        FirePlayerWeapon(world, player, wepdef);
    
}

bool WeaponSystem::HasPowerup(const GameWorld& world, PowerupType type) const
{
    for (const ActivePowerup& powerup : world.playerPowerups)
    {
        if(powerup.type == type)
        {
            return true;
        }
    }
    return false;
}

float WeaponSystem::GetPowerupValue(const GameWorld& world, PowerupType type, float defaultValue) const
{
     for(const ActivePowerup& powerup : world.playerPowerups)
     {
        if(powerup.type == type)
        {
            return powerup.value;
        }
     }
     return defaultValue;
}

void WeaponSystem::FirePlayerWeapon(GameWorld& world, Player& player, const WeaponDefinition& weapon)
 {
    const Vector2 spawnPosition = player.GetProjectileSpawnPos();

    //Projectile count

    uint8_t projectileCount = weapon.projectileCount;

    if (HasPowerup(world, PowerupType::TripleShot))
    {   
        //here we get the value of the powerup and cast it to a uint8_t to use as the projectile count
        projectileCount = static_cast<uint8_t>(GetPowerupValue(world, PowerupType::TripleShot, 3));
    }
    else if(HasPowerup(world, PowerupType::DoubleShot))
    {
        projectileCount = static_cast<uint8_t>(GetPowerupValue(world, PowerupType::DoubleShot, 2));
    }

    //Fire Rate

    const float fireRateMultiplier = GetPowerupValue(world, PowerupType::RapidFire, 1.0f);

    // We'll let Player own the actual cooldown.
   // Fire() is what resets that cooldown.
    player.SetShootInterval(1.f / (weapon.fireRate * fireRateMultiplier));

    //----------------Spawn--------------------
    for (uint8_t shot = 0; shot < projectileCount; ++shot)
    {
        Vector2 projectileDirection{ 0.f, -1.f }; // Default direction is straight up

        if (projectileCount > 1)
        {
            const float spread = 10.f;
            float offset = 0.f;

            if (projectileCount == 2)
            {
                offset = shot == 0 ? spread : -spread; //if there are 2 projectiles, one goes left and the other goes right
            }
            else if (projectileCount == 3)
            {
                if(shot==0)
                {
                    offset = -spread; // Left
                }
                else if(shot == 1)
                {
                    offset = 0.f; // Center
                }
                else
                {
                    offset = spread; // Right
                }
            }
            projectileDirection = Vector2Rotate(projectileDirection, offset * DEG2RAD); 
        }
        Projectile projectile{};
        projectile.owner = ProjectileOwner::Player;
        projectile.damage = weapon.damage;
        projectile.lifetime = 5.f;
        world.projectiles.push_back(projectile);
        world.projectilePositions.push_back(Position{ spawnPosition });
        world.projectileVelocities.push_back(Velocity{ Vector2Scale(projectileDirection, weapon.projectileSpeed) });
    }
    player.Fire(); // Reset the player's shooting cooldown
}
