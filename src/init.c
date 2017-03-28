/*
** init.c for Project-Master in /home/sellet_f/Projets/Tek2/PSU_2016_lemipc
**
** Made by sellet_f
** Login   <flavien.sellet@epitech.eu>
**
** Started on  Tue Mar 28 13:02:38 2017 sellet_f
** Last update	Tue Mar 28 16:28:17 2017 gastal_r
*/

#include "lemipc.h"

void    initMap(t_struct *core)
{
  int		x;
  int		y;
  int		i;

  x = -1;
  while (++x < 50)
    {
      y = -1;
      while (++y < 50)
	{
	  i = y * 50 + x;
	  core->addr->map[i] = EMPTY;
	}
    }
}

int		initSem(t_struct *core)
{
  if ((core->semId = semget(core->key, 1, SHM_R | SHM_W)) == -1)
  {
    if ((core->semId = semget(core->key, 1, IPC_CREAT | SHM_R | SHM_W)) == -1) /* get id ensemble semaphores OU en créée si inexistants */
      return (fprintf(stderr, "Semget failed\n") - 15);
    semctl(core->semId, 0, SETVAL, 1); /* SETVAL 1 sur semaphore 0 */
  }
  return (0);
}

int		initMsg(t_struct *core)
{
  if ((core->msgId = msgget(core->key, SHM_R | SHM_W)) == -1)
  {
    printf("Creation du msgID\n");
    if ((core->msgId = msgget(core->key, IPC_CREAT | SHM_R | SHM_W)) == -1)
      return (fprintf(stderr, "Msgget failed\n") - 15);
    }
  return (0);
}

int		initValues(t_struct *core, char *path, int idTeam)
{
  int		go_on;
  t_msg msg;
  t_player player;

  go_on = 1;
  if ((core->key = ftok(path, 0)) == -1)
    return (printUsage());
  initMsg(core);
  if (initSem(core) == -1)
    return (-1);
  printf("msgid %d\n", core->msgId);
  if ((core->shmId = shmget(core->key, sizeof(t_shared) * 50 * 50, SHM_R | SHM_W)) == -1)
    {
      if ((core->shmId = shmget(core->key, sizeof(t_shared) * 50 * 50, IPC_CREAT | SHM_R | SHM_W)) != -1)
	     if (initFirstPlayer(core, go_on, idTeam) == -1)
	      return (-1);
    }
  else
    {
      if (initOtherPlayers(core) == -1)
	     return (-1);
      initNewPlayer(core, &player, idTeam);
      mainloop(core, &player);
    }
  return (0);
}