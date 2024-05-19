/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*                      MAKRA                     */
#define CAPACITY                                (8U) //< Default pocet alokovanych mist v items.list
#define DIFF_BETWEEN_CAPITAL_AND_SMALL_LETTER   (32) 
#define ADD_ONE                                 (1)

void bst_update_or_add_value(bst_node_t **tree, char key, int val) {
    if (NULL == *tree) 
    {
        bst_insert(tree, key, val);
    } 
    else 
    {
        int Data = 0; // Default
        if (true == bst_search(*tree, key, &Data)) 
        {
            bst_insert(tree, key, Data + val);
        } 
        else 
        {
            bst_insert(tree, key, val);
        }
    }
}



/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    char key;
    // inicializace stromu
    bst_init(&(*tree));
    // Prevedeni retezce na LowerCase
    while ('\0' != *input) {
        if (('a' <= *input && 'z' >= *input) || ('A' <= *input && 'Z' >= *input)) 
        {
            if (('A' <= *input && 'Z'>= *input))
            {
                key = (*input) + DIFF_BETWEEN_CAPITAL_AND_SMALL_LETTER;   
            }
            else
            {
                key = (*input);
            }
        } 
        else if (' '== *input) 
        {
            key = ' ';
        } 
        else {
            // Ostatni znaky jsou reprezentovany -> '_'
            key = '_';
        }
        // Aktualizace uzlu s danym klicem
        bst_update_or_add_value(tree, key, ADD_ONE);
        // Posun na dalsi znak
        input++; 

    }
}
bst_node_t* array_to_bst(bst_node_t **ArrOfNodes, int Start, int End) {
    if (Start > End)
    {   // V tomto pripade jsme mimointerval a mame koncit 
        return NULL;
    }
    else
    {

        bst_node_t *RootOfTree = NULL;
        // Spocitat stred intervalu
        int MidNode = (Start + End) / 2;
        // Stred pole bude hlavni uzel
        RootOfTree = ArrOfNodes[MidNode];
        // Navazat na hlavni uzel jeho podstromy
        RootOfTree->left = array_to_bst(ArrOfNodes, Start, MidNode - 1);
        RootOfTree->right = array_to_bst(ArrOfNodes, MidNode + 1, End);
        
        return RootOfTree;
    }
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {   
    // Deklarace 
    bst_node_t *NewTree = NULL;
    bst_items_t items;
    int FirstNodeIndex;
    int LastNodeIndex;

    items.capacity = CAPACITY;      // pocatecni kapacita
    items.size = 0;                 // aktualni pocet polozek
    items.nodes = (bst_node_t**) malloc(sizeof(bst_node_t*)* CAPACITY);
    if (NULL == items.nodes)
    {
        // Chyba alokace prostoru
        return;
    }
    bst_inorder(*tree,&items);
    FirstNodeIndex = 0;             // Pocatecni index pole uzlu
    LastNodeIndex = items.size - 1; // Koncovy index pole uzlu

    NewTree = (bst_node_t*) array_to_bst(items.nodes,FirstNodeIndex,LastNodeIndex);
    if (NULL == NewTree)
    {
        return;
    }
    else
    {
        *tree = NewTree;
        free(items.nodes);
        return;
    }
}
