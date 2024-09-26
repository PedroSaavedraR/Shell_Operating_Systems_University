#include "files.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
/*
• Goal: to create a node by allocating memory
• Inputs: the variable for which memory will be allocated
• Outputs: TRUE if memory could be allocated for the new node and false otherwise
• Preconditions:
• Postconditions: */
bool createNode(tPos *p){
    *p = malloc(sizeof(struct tNode)); //allocates memory for a new node
    return *p != NULL; //if memory allocation was successful return true, else return false
}

/*
• Goal: to create an empty list
• Inputs: Name of the list
• Outputs: sets the address pointed to by L to NULL
• Preconditions: the name of the list must not have been previously used
• Postconditions: The list is initialised and has no elements */
void createfilelist (tfilelist *l){
    *l = NULL; // sets the address pointed to by L to NULL
}

/*
• Goal: to determine whether the list is empty or not
• Inputs: name of the list
• Outputs: TRUE if the list is empty and FALSE if not
• Preconditions: the list must exist
• Postconditions: */
bool isemptyfiles (tfilelist L){
    return (L==NULL); // if the first element is null, it is empty
}

tPos findPosition (tfile d, tfilelist L){
    tPos p;
    p=L;
    while ((p->next!=NULL)&&(strcmp(p->next->data.filename, d.filename)<0))
        p=p->next;
    return p;
}

/*
• Goal: to insert an element containing the provided data in the list
• Inputs: the item to insert, the address and the name of the list
• Outputs: true if the item was correctly inserted, false otherwise
• Preconditions: The specified address is a valid address in the list
• Postconditions: The elements in the list will rearrange */
// Function to add a file to the list
bool addfile(char *filename, int df, int mode, tfilelist *L) {
    tPos q, p;

    // Create a new node
    if (!createNode(&q)) // if malloc failed
        return false;

    // Initialize the new node's data
    strcpy(q->data.filename, filename);
    q->data.descriptor = df;
    q->data.mode = mode;
    q->next = NULL;

    // If the list is empty, insert at the front
    if (*L == NULL) {
        *L = q;
    }
        // If the new file should be inserted at the front (lexicographically smaller)
    else if (strcmp(filename, (*L)->data.filename) < 0) {
        q->next = *L;
        *L = q;
    }
    else {
        p = findPosition(q->data, *L); // Find the correct position
        q->next = p->next;
        p->next = q;
    }

    return true;
}


/*
• Goal: to retrieve the content of the element at the indicated address
• Inputs: address of the searched item and name of the list
• Outputs: the items at the given address (userName, numPlays and userCategory)
• Preconditions: The indicated address is a valid address in the list
• Postconditions:*/
tfile getfile (tPos p, tfilelist L){
    return p->data;
}

/*
• Goal: to return the address (L) of the first element of the list.
• Inputs: name of the list
• Outputs: returns the address of L, as it is at first position
• Preconditions: the list must not be empty
• Postconditions:*/
tPos first (tfilelist L){
    return L;
}

/*
• Goal: to return the address of the last element in the list
• Inputs: name of the list
• Outputs: address of the last item in the list
• Preconditions: the list must not be empty
• Postconditions:*/
tPos last (tfilelist L){
    tPos q;
    for (q = L; q->next != NULL; q=q->next); //iterating through the list until the last item
    return q;
}

/*
• Goal: to return the address preceding the one the user indicates (or NULLU if the specified address has no previous element).
• Inputs: address of the element and name of the list
• Outputs: the address of the previous item to the one inputted
• Preconditions: The indicated position must be a valid position in the list
• Postconditions:*/
tPos previous (tPos p, tfilelist L){
    tPos q;
    if (p==L) //if p is the first element
        return NULL; // as it is the first element, there is no previous
    else{
        for (q = L; q->next!=p; q=q->next); //searches in order until it reaches the element to find
        return q;
    }
}

/*
• Goal: to return the position next the one the user indicates (or NULLU if the specified position has no next element).
• Inputs: Position and name of the list
• Outputs: the address of the next item to the one inputted
• Preconditions: The indicated position must be a valid position in the list
• Postconditions:*/
tPos next (tPos p, tfilelist L){
    return p->next;
}

/*
• Goal: to return the address of the first element in the list whose username matches the one indicated (or NULL if there is no such element)
• Inputs: Username and name of the list
• Outputs: the address of the element or NULL if the list is empty or if the file is not stored in the list
• Preconditions: the list must be initialized
• Postconditions:*/
tPos findfile (char *d, tfilelist L){
    tPos p;
    for(p=L; (p!= NULL)&&(strcmp(d, p->data.filename)>0); p=p->next); //the comparison depends on tItem
    if(p!= NULL && strcmp(d,p->data.filename)==0)
        return p;
    else
        return NULL;
}

/*
• Goal: to delete the element at the given address from the list
• Inputs: address of the element to delete and name of the list
• Outputs: the indicated element is deleted and the elements in the node have rearranged correctly
• Preconditions: The indicated position is a valid position in the list and the playlist of its
associated user is empty
• Postconditions: The positions of the elements in the list following that of the
deleted one may have changed */
void deleteAtPosition (tPos p, tfilelist* L) {
    tPos q;

    if(p==*L) //delete the first
        *L = (*L)->next;
    else if (p->next == NULL) { // Deleting the last element
        for (q = *L; q->next != p; q = q->next); // Traverse to the node before p
        q->next = NULL; // Update next pointer
    } else { //deleting from the middle
        q=p->next; // p ----- q
        p->data = q->data; //data from p is now data from next to p
        p->next = q->next; //data from next to p is now data from p->next->next
        p=q; //get rid of the duplicated node

    }
    free(p); // p can now be used again
}

/*
• Goal: to modify the content of the element in the indicated address
• Inputs: The new item, the address where it will be inserted and the name of the list
• Outputs: the item is updated
• Preconditions: The indicated address is a valid address in the list
• Postconditions: The order of the elements in the list has not been modified */
void updatefile (tfile d, tPos p, tfilelist* L){
    p->data=d;
}

void printopenfiles(tfilelist l) {
    tPos p;
    if (!isemptyfiles(l)) {
        for (p = l; p->next != NULL; p = p->next) {
            printf("Name: %s\tDescriptor: %d\tMode: %d\n", p->data.filename, p->data.descriptor, p->data.mode);
        }
    } else printf("File list is empty");
}