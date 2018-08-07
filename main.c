/*Author: Wesley Luna, Harman Rai
* Date:7/25/2018
* Description: This is a Merkle Tree. The nodes
* will point to files which will be hashed using MD5
*/

#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

//Node for the Merkle Tree
struct Node{
    int data;
    unsigned char hdata[10024];
    int order;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
    struct Node *next;
    struct Node *prev;
    char *fp;
};

typedef struct Node Node;
typedef Node *NodePtr;

//Closes all the files
void closeFiles(NodePtr *a);
//Inserts a node
void insert(NodePtr *iptr, int value, char *fname);
//gets the top hash value
void topHash(NodePtr *Head);
//prints the list of nodes at a specific level
void printList(NodePtr *Head);
//fills the tree
void fillTree(NodePtr *Head);
//prints the tree
void printTree(NodePtr *Head);
//Makes a single hash
void singleHash(NodePtr *a);
//combines hashes for parent node
void doubleHash(NodePtr *a, NodePtr *b);
//Function for appending characters to array
unsigned char* append(unsigned char* s, unsigned char c);


int main(){
    int flag, i, maxNodes;
    NodePtr head = NULL;
    char *fname = (char *) malloc( 100 * sizeof( char ) );
    printf("Press 1 to insert a file, 2 to test, or -1 to quit.\n");
    scanf("%d", &flag);


    switch(flag){
        case 1:
            //to insert files
            printf("\nHow many file nodes do you want to create?\n");
            scanf("%d", &maxNodes);
            printf("\n");

            for(i = 1; i <= maxNodes; i++){
                printf("What is the name of file %d?\n", i);
                scanf("%s", fname);
                insert(&head, i, fname);
                printf("\n");
                fillTree(&head);
            }

            printTree(&head);
            topHash(&head);
            flag = 0;
            break;
        case 2:
            //To test the code
            test();
            flag = 0;
            break;
        case -1:
            //exited by user
            printf("\nProgram exited.");
            flag = -1;
            break;
        default:
            //exited because of invalid input
            printf("\nInvalid option. Program exited.");
            flag = 0;
            break;
    }

    return 0;
}

//Function used to test the code
int test()
{
    int i, maxNodes;
    NodePtr head = NULL;
    //files used to test the code
    char *filenamePtr = "file.c";
    char *filenamePtr2 = "file2.c";
    char *filenamePtr3 = "file3.c";
    char *filenamePtr4 = "file4.c";
    char *filenamePtr5 = "file5.c";
    char *filenamePtr6 = "file6.c";
    char *filenamePtr7 = "file7.c";
    char *filenamePtr8 = "file8.c";
    char *filenamePtr9 = "file9.c";
    char *filenamePtr10 = "file10.c";
    int temp;

    printf("\nYou are now testing with 10 files. How many file nodes do you want to create?\n");
    scanf("%d", &maxNodes);
    printf("\n");

    //Create i number of nodes using 10 files
    for(i = 1; i <= maxNodes; i++){
        if(i%10==0){
            insert(&head, i, filenamePtr10);
        }else if(i%10==1){
            insert(&head, i, filenamePtr9);
        }else if(i%10==2){
            insert(&head, i, filenamePtr8);
        }else if(i%10==3){
            insert(&head, i, filenamePtr7);
        }else if(i%10==4){
            insert(&head, i, filenamePtr6);
        }else if(i%10==5){
            insert(&head, i, filenamePtr5);
        }else if(i%10==6){
            insert(&head, i, filenamePtr4);
        }else if(i%10==7){
            insert(&head, i, filenamePtr3);
        }else if(i%10==8){
            insert(&head, i, filenamePtr2);
        }else{
            insert(&head, i, filenamePtr);
        }

        fillTree(&head);
    }

    //prints the tree and top hash
    printf("\nThe Tree\n\n");
    printTree(&head);
    topHash(&head);
    return 0;
}

//Inserts linked list node in ascending order
void insert(NodePtr *iptr, int value, char *fname){

    NodePtr newPtr; // new node
    NodePtr prev; // previous node
    NodePtr cPtr; // current node
    int i;
    newPtr = malloc( sizeof( Node ) );


    if ( newPtr ) {

        newPtr->data = value;
        newPtr->fp= fname;
        newPtr->order = value;
        newPtr->next = NULL;
        newPtr->prev = NULL;
        singleHash(&newPtr);
        prev = NULL;
        cPtr = *iptr;

        while(cPtr != NULL && value > cPtr->data ){
            prev = cPtr;
            cPtr = cPtr->next;
        }
        if ( prev == NULL ) {
            newPtr->next = *iptr;
            *iptr = newPtr;
        }else {
            prev->next = newPtr;
            newPtr->prev = prev;
            newPtr->next = cPtr;
        }

    }else {
        printf( "%c not inserted. No memory available.\n", value );
    }
}

//prints the top hash
void topHash(NodePtr *Head){
    int i;
    NodePtr cPtr = *Head;
    while(cPtr->parent != NULL)
        cPtr= cPtr->parent;

    printf("\n\nTop Hash\n%d ", cPtr->data);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", cPtr->hdata[i]);
    printf("\n");
}

//Prints the tree
void printTree(NodePtr *Head){
    int y;
    int i = 1;
    NodePtr cPtr = *Head;
    NodePtr start;
    printf("\nLevel 1\n");
    while(cPtr->parent != NULL){
        start = cPtr;
        printf("%d ", cPtr->data);
        for(y = 0; y < MD5_DIGEST_LENGTH; y++) printf("%02x", cPtr->hdata[y]);
        printf("\n");
        while(cPtr->next != NULL){
            cPtr = cPtr->next;
            printf("%d ", cPtr->data);
            for(y = 0; y < MD5_DIGEST_LENGTH; y++) printf("%02x", cPtr->hdata[y]);
            printf("\n");
        }
        i++;
        printf("\nLevel %d\n", i);
        cPtr = start->parent;
    }
    printf("%d ", cPtr->data); //for top Node
    for(y = 0; y < MD5_DIGEST_LENGTH; y++) printf("%02x", cPtr->hdata[y]);
}

//Fills the tree with nodes and the correct information
void fillTree(NodePtr *Head){
    int i;
    NodePtr cPtr = *Head;
    NodePtr Prev;
    NodePtr newPtr;
    NodePtr start = cPtr;
    int x = 1;
    if(cPtr->next == NULL){
        newPtr = malloc( sizeof( Node ) );
        cPtr->parent = newPtr;
        newPtr->data = cPtr->data;
        for(i = 0; i < MD5_DIGEST_LENGTH; i++) newPtr->hdata[i] =  cPtr->hdata[i];
        newPtr->left = cPtr;
        newPtr->order = x;
    }

    while(cPtr->next != NULL){

            while(cPtr->next != NULL){
                Prev = cPtr;
                cPtr = cPtr->next;
                if(cPtr->order%2==1 ){
                        x++;
                        if(cPtr->parent==NULL){
                                newPtr = malloc( sizeof( Node ) );
                                cPtr->parent = newPtr;
                                newPtr->left = cPtr;
                                newPtr->order = x;
                                newPtr->data = cPtr->data;
                                for(i = 0; i < MD5_DIGEST_LENGTH; i++) newPtr->hdata[i] =  cPtr->hdata[i];
                                newPtr->prev = Prev->parent;
                                Prev->parent->next= newPtr;
                        }else{
                            cPtr->parent->data = cPtr->data;
                            for(i = 0; i < MD5_DIGEST_LENGTH; i++) cPtr->parent->hdata[i] =  cPtr->hdata[i];
                            cPtr->parent->prev = Prev->parent;
                            Prev->parent->next = cPtr->parent;
                        }
                }else{
                    newPtr = Prev->parent;
                    cPtr->parent = newPtr;
                    newPtr->right = cPtr;
                    newPtr->left = Prev;
                    newPtr->data = Prev->data + cPtr->data;
                    doubleHash(&newPtr->right, &newPtr->left);
                }
            }
            start = start->parent;
            cPtr = start;
            if(cPtr->next != NULL){
                x = 1;
                newPtr = malloc( sizeof( Node ) );
                cPtr->parent = newPtr;
                newPtr->data = cPtr->data;
                for(i = 0; i < MD5_DIGEST_LENGTH; i++) newPtr->hdata[i] =  cPtr->hdata[i];
                newPtr->left = cPtr;
                newPtr->order = x;
            }
    }
}

//prints the specified level of the tree
void printList(NodePtr *Head){
    printf("\nPrint list\n");
    NodePtr cPtr = *Head;
    NodePtr Prev;
    while(cPtr!= NULL){
        printf("%d ", cPtr->data);
        Prev = cPtr;
        cPtr = cPtr->next;
    }

    printf("\nThe reverse order\n");
    while(Prev != NULL){
        printf("%d ", Prev->data);
        Prev = Prev->prev;
    }
}

void singleHash(NodePtr *a){
    NodePtr x = *a;
    //First File String
    unsigned char c[10024];

    //Sets File Chars
    char *filename= x->fp;
    //Counter
    int i;
    //Hash for first file
    MD5_CTX mdContext;

    //Bytes of each file that we hash
    int bytes;

    //Data to save bytes
    unsigned char data[10024];

    //Opening files
    FILE *inFile = fopen (filename, "rb");


    //Checks if either are empty or don't exist
     if (inFile == NULL) {
        printf ("%s can't be opened.\n", filename);
        return 0;
    }

    MD5_Init (&mdContext);

    //Reads data byte by byte
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
        MD5_Update (&mdContext, data, bytes);
    MD5_Final (c,&mdContext);

    for(i = 0; i < MD5_DIGEST_LENGTH; i++) x->hdata[i] =  c[i];

    fclose (inFile);

    //ends
}

//Main reads both files
void doubleHash(NodePtr *a, NodePtr *b){
    NodePtr x = *a;
    NodePtr y = *b;
    if(x==NULL || y==NULL){
        printf("\nchildren are null\n");
    }
    unsigned char combined[10024];

    unsigned char finalH[10024];
    //Sets File Chars
    char *filename= x->fp;
    char *filename2= y->fp;
    //Counter
    int i;
    //Hash for parent node
    MD5_CTX mdContext;
    unsigned char data[10024];

    for(i = 0; i < MD5_DIGEST_LENGTH; i++) append(combined,x->hdata[i]);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) append(combined,y->hdata[i]);

    //turns combined into hash
    MD5_Init (&mdContext);
    //Reads data byte by byte
    MD5_Update (&mdContext, combined, strlen(combined));
    MD5_Final (finalH,&mdContext);

    for(i = 0; i < MD5_DIGEST_LENGTH; i++) x->parent->hdata[i] =  finalH[i];

    //ends
}

//Function for appending characters to array
unsigned char* append(unsigned char* s, unsigned char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
        return *s;
}
