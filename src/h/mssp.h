struct mssp_info
{
  char                   *hostname;
  char                   *contact;
  char                   *icon;
  char                   *language;
  char                   *location;
  char                   *website;
  char                   *family;
  char                   *genre;
  char                   *gamePlay;
  char                   *gameSystem;
  char                   *intermud;
  char                   *status;
  char                   *subgenre;
  char                   *equipmentSystem;
  char                   *multiplaying;
  char                   *playerKilling;
  char                   *questSystem;
  char                   *roleplaying;
  char                   *trainingSystem;
  char                   *worldOriginality;
  short                   created;
  short                   minAge;
  short                   worlds;
  bool                    ansi;
  bool                    mccp;
  bool                    mcp;
  bool                    msp;
  bool                    ssl;
  bool                    mxp;
  bool                    pueblo;
  bool                    vt100;
  bool                    xterm256;
  bool                    pay2play;
  bool                    pay4perks;
  bool                    hiringBuilders;
  bool                    hiringCoders;
  bool                    adultMaterial;
  bool                    multiclassing;
  bool                    newbieFriendly;
  bool                    playerCities;
  bool                    playerClans;
  bool                    playerCrafting;
  bool                    playerGuilds;
};

#define MSSP_FILE SYSTEM_DIR "mssp.dat"

#define MSSP_MINAGE  0
#define MSSP_MAXAGE  21

#define MSSP_MINCREATED 1970
#define MSSP_MAXCREATED 2100

#define MSSP_MAXVAL 20000
#define MAX_MSSP_VAR1 4
#define MAX_MSSP_VAR2 3

bool                    load_mssp_data(void);
void                    send_mssp_data(DESCRIPTOR_DATA *d);
void                    free_mssp_info(void);
