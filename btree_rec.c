/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
    if (NULL != tree)
    {
        *tree = NULL;
    }

}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (NULL == tree)
    {
        return false;
    }
    else
    {
        if (key == tree->key)
        {
            *value = tree->value;
            return true;
        }
        else
        {
            if(key < tree->key)
            {
                return (bst_search(tree->left,key,value));
            }
            else
            {
                return (bst_search(tree->right,key,value));
            }
        }
    }
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if (NULL == *tree)
    {   // vytvoreni korenoveho uzlu nebo normalniho uzlu
        *tree = (bst_node_t*) malloc(sizeof(bst_node_t));
        if (NULL == tree)
        {
            // Problem s alokaci 
            return;
        }
        else
        {   // prirazeni hodnot
            (*tree)->key    = key;
            (*tree)->value  = value;
            (*tree)->left   = NULL;
            (*tree)->right  = NULL;
        }
    }
    else if (key < (*tree)->key)
    {
        // pokud je klic mensi nez klic aktualniho uzlu
        bst_insert(&((*tree))->left,key,value); 
    }
    else if (key > (*tree)->key)
    {
        // pokud je klic vetsi nez klic aktualniho uzlu
        bst_insert(&((*tree))->right,key,value);
    }
    else
    {
        // jinak se klice rovnaji 
        (*tree)->value = value;;
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if (NULL == (*tree)->right)
    {
        target->key = (*tree)->key;
        target->value = (*tree)->value;

        // Uzel ktery chceme odstranit
        bst_node_t *NodeToBeDeleted = *tree;
        *tree = (*tree)->left;

        free(NodeToBeDeleted);
        return;
    }
    else
    {
        bst_replace_by_rightmost(target, &((*tree)->right));
    }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (NULL == (*tree))
    {   // Nemame co mazat 
        return;
    }
    if (key > (*tree)->key)
    {   // Jdeme do prava jelikoz je hodnata klice vetsi jak hodnota akt. uzlu 
        bst_delete(&(*tree)->right,key);
    }   
    else if(key <(*tree)->key)
    {   // Hodnota klice je mensi -> jdi doleva
        bst_delete(&(*tree)->left,key);
    } 
    else
    {   // Jinak je hodnoty klicu rovnaji 
        bst_node_t *LeftChild = NULL;   // Odkaz na leveho potomka
        bst_node_t *RightChild = NULL;  // Odkaz na praveho potomka
        
        // Pokud ma odstraneny uzel jeden podstrom, zdedi ho rodic odstraneneho uzlu
        if (NULL == (*tree)->left)
        {   // Situace kdyz strom ma pouze pravy podstrom
            RightChild = (*tree)->right;
            free(*tree);
            *tree = RightChild; 
        }
        else if (NULL == (*tree)->right)
        {   // Situace kdy ma strom pouze levy podstrom
            LeftChild = (*tree)->left;
            free(*tree);
            *tree = LeftChild;
        }
        else
        {   // Jinak ma odstranovany uzel oba podstromy
            // tzn. je treba ho nahradit nejpravejsim uzlem leveho podstromu
            bst_replace_by_rightmost(*tree,&((*tree)->left));
            // Odstraneni puvodniho nejpravejsiho
            bst_delete(&((*tree)->left),(*tree)->key);

        }
    }
 
}

/*
 * Zrušení celého stromu.
 * ?
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    if (NULL != (*tree))
    {
        bst_dispose(&(*tree)->left);
        bst_dispose(&(*tree)->right);
        free(*tree);
        *tree = NULL;
    }

}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    if(NULL != tree)
    {
        bst_add_node_to_items(tree,items);
        bst_preorder(tree->left,items);
        bst_preorder(tree->right,items);

    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    if(NULL != tree)
    {
        bst_inorder(tree->left,items);
        bst_add_node_to_items(tree,items);
        bst_inorder(tree->right,items);
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    if(NULL != tree)
    {
        bst_postorder(tree->left,items);
        bst_postorder(tree->right,items);
        bst_add_node_to_items(tree,items);
    }
}
