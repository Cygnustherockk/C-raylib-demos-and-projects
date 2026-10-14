#include "w_guns.h"

static clock_t last_shot = 0;

GameGun LoadGameGun(ItemCollection itemCollection)
{
  GameGun temp_gun = {0};
  for(int i = 0; i < itemCollection.count; i++)
  {
    if(strcmp(itemCollection.item_data[i].item_name, PARSE_DAMAGE) == 0) {temp_gun.damage = atoi(itemCollection.item_data[i].item_value);}
    if(strcmp(itemCollection.item_data[i].item_name, PARSE_FIRERATE) == 0) {temp_gun.firerate = atoi(itemCollection.item_data[i].item_value);}
    if(strcmp(itemCollection.item_data[i].item_name, PARSE_RELOADSPEED) == 0) {temp_gun.reloadSpeed = atoi(itemCollection.item_data[i].item_value);}
  }
  return temp_gun;
}

bool ShootGun(Camera camera, Vector3 playerPos)
{
  Ray worldRay = GetScreenToWorldRay(GetMousePosition(), camera);
  worldRay.direction = Vector3Scale(worldRay.direction, 100);
  DrawRay(worldRay, RED);
  last_shot = clock();
  if(GetRayCollisionBox(worldRay, (BoundingBox){(Vector3){playerPos.x - 2.5, playerPos.y - 5, playerPos.z - 2.5}, (Vector3){playerPos.x + 2.5, playerPos.y + 5, playerPos.z + 2.5}}).hit) return true;
  return false;
}

bool CanShoot(GameGun currentGun)
{
  if(currentGun.gunState != GUN_STATE_RELOAD && (clock() - last_shot) >= currentGun.firerate)return true;
  else return false;
}