/*
** main.c for PSU_2016_lempipc
**
** Made by	gastal_r
** Login	gastal_r
**
** Started on	Sun Mar 26 12:28:14 2017 gastal_r
** Last update	Thu Mar 30 23:04:45 2017 gastal_r
*/

#include      "lemipc.h"


void		freeIPCS(t_struct *core)
{
  shmctl(core->key, IPC_RMID, NULL);
  semctl(core->key, 1, IPC_RMID);
  msgctl(core->key, IPC_RMID, NULL);
}


int		checkNewTeam(t_struct *core, int idTeam)
{
  int		i;

  i = -1;
  while (++i < 2500)
    if (core->addr->map[i] == idTeam)
      return (0);
  return (1);
}

void		semOperation(t_struct *core, int op)
{
  struct sembuf	sops;

  sops.sem_num = 0;
  sops.sem_flg = 0;
  sops.sem_op = op;
  semop(core->semId, &sops, 1);
}

int		main(int ac, char *av[])
{
  t_struct	core;

  if (ac != 3)
    return (printUsage());
  core.addr = NULL;
  srand(time(NULL));
  if (initValues(&core, av[1], atoi(av[2])) == -1)
    return (-1);
  return (0);
}
