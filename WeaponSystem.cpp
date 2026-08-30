#include "WeaponSystem.h"
#include "raylib.h"
#include "raymath.h"

void WeaponSystem::Update(GameWorld& world, const EnemyDatabase& enemyDatabase, const WeaponDatabase& weaponDatabase, Vector2 playerPos, float deltaTime)
{
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
}
