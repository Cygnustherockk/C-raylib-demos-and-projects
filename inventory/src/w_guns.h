#ifndef _GAME_GUNS_H_
#define _GAME_GUNS_H_
#include "raylib.h"
#include <stdio.h>
#include "Third-Party/raymath.h"
#include <time.h>
#include "f_parser.h"

#define PARSE_DAMAGE "Damage"
#define PARSE_FIRERATE "Firerate"
#define PARSE_RELOADSPEED "ReloadSpeed"

typedef enum{
  GUN_STATE_IDLE,
  GUN_STATE_SHOOT,
  GUN_STATE_RELOAD,
  GUN_STATE_EQUIP
}GunState;

typedef struct GameGun{
  Model model;
  ModelAnimation* modelAnimations;
  int damage;
  int firerate;
  int reloadSpeed;
  int gunState;
  int maxAmmo;
  int totalAmmo;
  int currentAmmo;
}GameGun;

typedef struct GameGunCollection{
  int count;
  int current_gun;
  GameGun* gameGuns;
}GameGunCollection;

bool ShootGun(Camera camera, Vector3 playerPos);
GameGun LoadGameGun(ItemCollection itemCollection);
bool CanShoot(GameGun currentGun);
#endif