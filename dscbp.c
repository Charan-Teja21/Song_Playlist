#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
    #include <windows.h>
    void msleep(unsigned int ms) { Sleep(ms); }
    void sleep_seconds(unsigned int s) { Sleep(s * 1000); }
#else
    #include <unistd.h>
    void msleep(unsigned int ms) { usleep(ms * 1000); }
    void sleep_seconds(unsigned int s) { sleep(s); }
#endif
#define PLAYLIST_FILE "playlist.txt"
struct Song
{
    char title[100];
    char artist[100];
    int duration;
    char filepath[256];
    struct Song *next;
} *first = NULL, *last = NULL;
int m = 0;
int generateRandomNumber(int seed, int max)
{
    return ((seed + 7) * seed) % max;
}
void savePlaylist() {
    FILE *fp = fopen(PLAYLIST_FILE, "w");
    if (!fp) return;
    struct Song *temp = first;
    while (temp) {
        fprintf(fp, "%s|%s|%d|%s\n", temp->title, temp->artist, temp->duration, temp->filepath);
        temp = temp->next;
    }
    fclose(fp);
}
void loadPlaylist() {
    FILE *fp = fopen(PLAYLIST_FILE, "r");
    if (!fp) return;
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        char *title = strtok(line, "|");
        char *artist = strtok(NULL, "|");
        char *durationStr = strtok(NULL, "|");
        char *filepath = strtok(NULL, "|");
        if (!title || !artist || !durationStr || !filepath) continue;
        struct Song *nn = (struct Song *)malloc(sizeof(struct Song));
        strncpy(nn->title, title, 99); nn->title[99] = 0;
        strncpy(nn->artist, artist, 99); nn->artist[99] = 0;
        nn->duration = atoi(durationStr);
        strncpy(nn->filepath, filepath, 255); nn->filepath[255] = 0;
        nn->next = NULL;
        if (first == NULL) first = nn;
        else last->next = nn;
        last = nn;
        m++;
    }
    fclose(fp);
}
void shufflePlaylist()
{
    if (first == NULL)
        return;
    struct Song **songArray = (struct Song **)malloc(m * sizeof(struct Song *));
    if (songArray == NULL)
        return;
    struct Song *temp = first;
    int i = 0;
    while (temp != NULL)
    {
        songArray[i++] = temp;
        temp = temp->next;
    }
    for (i = m - 1; i > 0; i--)
    {
        int j = generateRandomNumber(i, m);
        struct Song *tmp = songArray[i];
        songArray[i] = songArray[j];
        songArray[j] = tmp;
    }
    first = songArray[0];
    last = songArray[m - 1];
    for (i = 0; i < m - 1; i++)
        songArray[i]->next = songArray[i + 1];
    songArray[m - 1]->next = NULL;
    free(songArray);
    savePlaylist();
}
void sortTitle()
{
    if (first == NULL)
        return;
    struct Song *i, *j;
    for (i = first; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (strcmp(i->title, j->title) > 0)
            {
                char tempTitle[100], tempArtist[100];
                int tempDuration;
                char tempFilepath[256];
                strcpy(tempTitle, i->title);
                strcpy(tempArtist, i->artist);
                tempDuration = i->duration;
                strcpy(tempFilepath, i->filepath);
                strcpy(i->title, j->title);
                strcpy(i->artist, j->artist);
                i->duration = j->duration;
                strcpy(i->filepath, j->filepath);
                strcpy(j->title, tempTitle);
                strcpy(j->artist, tempArtist);
                j->duration = tempDuration;
                strcpy(j->filepath, tempFilepath);
            }
        }
    }
    savePlaylist();
}
void sortArtist()
{
    if (first == NULL)
        return;
    struct Song *i, *j;
    for (i = first; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (strcmp(i->artist, j->artist) > 0)
            {
                char tempTitle[100], tempArtist[100];
                int tempDuration;
                char tempFilepath[256];
                strcpy(tempTitle, i->title);
                strcpy(tempArtist, i->artist);
                tempDuration = i->duration;
                strcpy(tempFilepath, i->filepath);
                strcpy(i->title, j->title);
                strcpy(i->artist, j->artist);
                i->duration = j->duration;
                strcpy(i->filepath, j->filepath);
                strcpy(j->title, tempTitle);
                strcpy(j->artist, tempArtist);
                j->duration = tempDuration;
                strcpy(j->filepath, tempFilepath);
            }
        }
    }
    savePlaylist();
}
void addDefaultSongs() {
    struct stat st;
    if (stat(PLAYLIST_FILE, &st) == 0 && st.st_size > 0) return; // Only add if file doesn't exist or is empty
    struct Song *nn;
    nn = (struct Song *)malloc(sizeof(struct Song));
    strcpy(nn->title, "Shape of You");
    strcpy(nn->artist, "Ed Sheeran");
    nn->duration = 240;
    strcpy(nn->filepath, "uploads/song1.mp3");
    nn->next = NULL;
    if (first == NULL) first = nn;
    else last->next = nn;
    last = nn;
    m++;
    nn = (struct Song *)malloc(sizeof(struct Song));
    strcpy(nn->title, "Blinding Lights");
    strcpy(nn->artist, "The Weeknd");
    nn->duration = 200;
    strcpy(nn->filepath, "uploads/song2.mp3");
    nn->next = NULL;
    last->next = nn;
    last = nn;
    m++;
    nn = (struct Song *)malloc(sizeof(struct Song));
    strcpy(nn->title, "Levitating");
    strcpy(nn->artist, "Dua Lipa");
    nn->duration = 220;
    strcpy(nn->filepath, "uploads/song3.mp3");
    nn->next = NULL;
    last->next = nn;
    last = nn;
    m++;
    savePlaylist();
}
void printSongs() {
    struct Song *temp = first;
    int idx = 1;
    printf("SONGS|%d", m);
    while (temp) {
        printf("|%d|%s|%s|%d|%s", idx, temp->title, temp->artist, temp->duration, temp->filepath);
        temp = temp->next;
        idx++;
    }
    printf("\n");
    fflush(stdout);
}
void processCommand(char *cmd) {
    if (strncmp(cmd, "LIST", 4) == 0) {
        printSongs();
    } else if (strncmp(cmd, "ADD|", 4) == 0) {
        char *title = strtok(cmd + 4, "|");
        char *artist = strtok(NULL, "|");
        char *durationStr = strtok(NULL, "|");
        char *filepath = strtok(NULL, "|");
        if (!title || !artist || !durationStr || !filepath) {
            printf("ERR|Invalid ADD command\n");
            fflush(stdout);
            return;
        }
        struct Song *nn = (struct Song *)malloc(sizeof(struct Song));
        strncpy(nn->title, title, 99); nn->title[99] = 0;
        strncpy(nn->artist, artist, 99); nn->artist[99] = 0;
        nn->duration = atoi(durationStr);
        strncpy(nn->filepath, filepath, 255); nn->filepath[255] = 0;
        nn->next = NULL;
        if (first == NULL) first = nn;
        else last->next = nn;
        last = nn;
        m++;
        savePlaylist();
        printf("OK|Song added\n");
        fflush(stdout);
    } else if (strncmp(cmd, "DELETE|", 7) == 0) {
        char *title = cmd + 7;
        struct Song *temp = first, *prev = NULL;
        while (temp) {
            if (strcmp(temp->title, title) == 0) {
                if (prev == NULL) first = temp->next;
                else prev->next = temp->next;
                if (temp == last) last = prev;
                free(temp);
                m--;
                savePlaylist();
                printf("OK|Song deleted\n");
                fflush(stdout);
                return;
            }
            prev = temp;
            temp = temp->next;
        }
        printf("ERR|Song not found\n");
        fflush(stdout);
    } else if (strncmp(cmd, "PLAY|", 5) == 0) {
        int idx = atoi(cmd + 5);
        struct Song *temp = first;
        int cur = 1;
        while (temp && cur < idx) { temp = temp->next; cur++; }
        if (!temp) {
            printf("ERR|Song not found\n");
            fflush(stdout);
            return;
        }
        printf("OK|Playing|%s|%s|%d|%s\n", temp->title, temp->artist, temp->duration, temp->filepath);
        fflush(stdout);
        char syscmd[512];
    #ifdef _WIN32
        snprintf(syscmd, sizeof(syscmd), "start \"\" \"%s\"", temp->filepath);
    #elif __APPLE__
        snprintf(syscmd, sizeof(syscmd), "afplay \"%s\"", temp->filepath);
    #else
        snprintf(syscmd, sizeof(syscmd), "aplay \"%s\"", temp->filepath);
    #endif
        system(syscmd);
    } else if (strncmp(cmd, "SHUFFLE", 7) == 0) {
        shufflePlaylist();
        printf("OK|Playlist shuffled\n");
        fflush(stdout);
    } else if (strncmp(cmd, "SORT_TITLE", 10) == 0) {
        sortTitle();
        printf("OK|Playlist sorted by title\n");
        fflush(stdout);
    } else if (strncmp(cmd, "SORT_ARTIST", 11) == 0) {
        sortArtist();
        printf("OK|Playlist sorted by artist\n");
        fflush(stdout);
    } else {
        printf("ERR|Unknown command\n");
        fflush(stdout);
    }
}
int main() {
    loadPlaylist();
    addDefaultSongs();
    char buf[1024];
    while (fgets(buf, sizeof(buf), stdin)) {
        buf[strcspn(buf, "\n")] = 0;
        if (strcmp(buf, "EXIT") == 0) break;
        processCommand(buf);
    }
    struct Song *temp = first;
    while (temp) {
        struct Song *nextSong = temp->next;
        free(temp);
        temp = nextSong;
    }
    return 0;
}