#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Song
{
    char title[100];
    char artist[100];
    int duration;
    struct Song *next;
} *first = NULL, *last = NULL;
int m = 0;
int generateRandomNumber(int seed, int max)
{
    return ((seed + 7) * seed) % max;
}
void create()
{
    struct Song *nn = (struct Song *)malloc(sizeof(struct Song));
    printf("Enter song title: ");
    fflush(stdin);
    gets(nn->title);
    printf("Enter artist name: ");
    fflush(stdin);
    gets(nn->artist);
    fflush(stdin);
    printf("Enter duration (in seconds): ");
    scanf("%d", &nn->duration);
    nn->next = NULL;
    if (first == NULL)
    {
        first = nn;
    }
    else
    {
        last->next = nn;
    }
    last = nn;
    m += 1;
    printf("Song \"%s\" by %s added to the playlist.\n", nn->title, nn->artist);
}
void Delete()
{
    if (first == NULL)
    {
        printf("Playlist is empty.\n");
        return;
    }
    char p[100];
    printf("Enter the song title to delete: ");
    fflush(stdin);
    gets(p);
    fflush(stdin);
    struct Song *temp = first, *prev = NULL;
    while (temp != NULL)
    {
        if (strcmp(temp->title, p) == 0)
        {
            printf("Song \"%s\" deleted from the playlist.\n", temp->title);
            if (prev == NULL)
            {
                first = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            if (temp == last)
            {
                last = prev;
            }
            free(temp);
            m--;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Song \"%s\" not found in the playlist.\n", p);
}
void display()
{
    struct Song *temp = first;
    if (temp == NULL)
    {
        printf("Playlist is empty.\n");
        return;
    }
    int count = 1;
    while (temp != NULL)
    {
        printf("%d. \"%s\" by %s [Duration: %d seconds]\n", count, temp->title, temp->artist, temp->duration);
        temp = temp->next;
        count++;
    }
}
void shufflePlaylist()
{
    if (first == NULL)
    {
        printf("Playlist is empty.\n");
        return;
    }
    struct Song **songArray = (struct Song **)malloc(m * sizeof(struct Song *));
    if (songArray == NULL)
    {
        printf("Memory allocation error.\n");
        return;
    }
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
        struct Song *temp = songArray[i];
        songArray[i] = songArray[j];
        songArray[j] = temp;
    }
    first = songArray[0];
    last = songArray[m - 1];
    for (i = 0; i < m - 1; i++)
    {
        songArray[i]->next = songArray[i + 1];
    }
    songArray[m - 1]->next = NULL;
    free(songArray);
    printf("Playlist shuffled.\n");
}
void sortTitle()
{
    if (first == NULL)
    {
        printf("Playlist is empty.\n");
        return;
    }
    
    struct Song *i, *j;
    for (i = first; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (strcmp(i->title, j->title) > 0)
            {
                // Swap song data
                char tempTitle[100];
                char tempArtist[100];
                int tempDuration;
                
                strcpy(tempTitle, i->title);
                strcpy(tempArtist, i->artist);
                tempDuration = i->duration;
                
                strcpy(i->title, j->title);
                strcpy(i->artist, j->artist);
                i->duration = j->duration;
                
                strcpy(j->title, tempTitle);
                strcpy(j->artist, tempArtist);
                j->duration = tempDuration;
            }
        }
    }
    printf("Playlist sorted by title.\n");
}

void sortArtist()
{
    if (first == NULL)
    {
        printf("Playlist is empty.\n");
        return;
    }
    
    struct Song *i, *j;
    for (i = first; i != NULL; i = i->next)
    {
        for (j = i->next; j != NULL; j = j->next)
        {
            if (strcmp(i->artist, j->artist) > 0)
            {
                // Swap song data
                char tempTitle[100];
                char tempArtist[100];
                int tempDuration;
                
                strcpy(tempTitle, i->title);
                strcpy(tempArtist, i->artist);
                tempDuration = i->duration;
                
                strcpy(i->title, j->title);
                strcpy(i->artist, j->artist);
                i->duration = j->duration;
                
                strcpy(j->title, tempTitle);
                strcpy(j->artist, tempArtist);
                j->duration = tempDuration;
            }
        }
    }
    printf("Playlist sorted by artist name.\n");
}

int main()
{
    int choice;
    do
    {
        printf("\n--------- Song Playlist Menu ---------\n");
        printf("1. Add a Song\n");
        printf("2. Delete a Song\n");
        printf("3. Shuffle Playlist\n");
        printf("4. Sort Playlist by Title\n");
        printf("5. Sort Playlist by Artist\n");
        printf("6. Display Playlist\n");
        printf("7. Exit\n");
        printf("---------------------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        switch (choice)
        {
        case 1:
            create();
            break;
        case 2:
            Delete();
            break;
        case 3:
            shufflePlaylist();ß
            break;
        case 4:
            sortTitle();
            break;
        case 5:
            sortArtist();
            break;
        case 6:
            display();
            break;
        case 7:
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
    struct Song *temp = first;
    while (temp != NULL)
    {
        struct Song *nextSong = temp->next;
        free(temp);
        temp = nextSong;
    }
    return 0;
}