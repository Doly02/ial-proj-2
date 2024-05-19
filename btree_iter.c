/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
    if (tree != NULL) 
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    //TODO:
    while (NULL != tree) 
    {
        if (key < tree->key) 
        {
            // Pokud je hledany klic mensi, jdeme doleva
            tree = tree->left;
        } 
        else if (key > tree->key) 
        {
            // Pokud je hledany klic vetsi, jdeme doprava
            tree = tree->right;
        } 
        else 
        {
            // Nasli jsme uzel s danym klicem 
            if (NULL != value) {
                *value = tree->value; // Nastaveni hodnoty
            }
            return true; // uspech
        }
    }
    return false;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    // Kontrola zda nejaky strom existuje
    if (NULL == *tree) {
        // Pokud je strom prazdny -> vytvarime korenovy uzel
        *tree = (bst_node_t*) malloc(sizeof(bst_node_t));
        if (NULL == *tree) 
        {
            // Nepovedena alokace pro novy uzel
            return;
        }
        (*tree)->key = key;
        (*tree)->value = value;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        return;
    }
    // Iterace stromem dokud nenajdeme vhodne misto
    while (NULL != *tree)
    {
        if (key < (*tree)->key)
        {
            tree = &((*tree)->left); //< Pokud je klic mensi tak jdeme doleva
        }
        else if (key > (*tree)->key)
        {
            tree = &((*tree)->right); //< Pokud je klic vetsi tak jdeme doprava
        }
        else
        {
            // Nasli jsme existujici uzel s klicem, takze nahrazujeme hodnotu
            (*tree)->value = value; 
            return;
        }
    }
    // Pokud jsme nanasli uzel se stejnym klicem
    bst_node_t *NewNode = (bst_node_t*) malloc(sizeof(bst_node_t));
    if (NULL == NewNode)
    {
        // osetreni chyb
        bst_dispose(&(*tree)); 
        return;
    } 
    // Prirazeni hodnot noveho uzlu
    NewNode->key      = key;
    NewNode->value    = value;
    NewNode->left     = NULL;
    NewNode->right    = NULL;

    *tree = NewNode;

}
  


/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if(NULL == tree || NULL == *tree || NULL == target)
    {
        return;
    }
    
    bst_node_t *ParentToRightMost = NULL;   // Rodic nejpravejsiho uzlu
    bst_node_t *RightMost = *tree;          // Nejpravejsi uzel

    // Najdi nejpravdivejsi uzel
    while (NULL != RightMost->right)
    {
        // Aktualizace rodice
        ParentToRightMost = RightMost;
        // Posun doprava
        RightMost = RightMost->right;
    }
    // Nahrazeni klice a hodnotu ciloveho uzlu
    target->key   = RightMost->key;
    target->value = RightMost->value;

    // Odstanit nejpravejsi uzel
    if (NULL != ParentToRightMost)
    {
        ParentToRightMost->right = RightMost->left;
    }
    else
    {
        *tree = RightMost->left;
    }

    free(RightMost);
    return;
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (NULL == tree || NULL == *tree) {
        // Pokud je strom prazdny nebo ukazatel na strom je NULL, nemame co mazat
        return;
    }

    bst_node_t *parent = NULL; // Rodic aktualniho uzlu uzlu
    bst_node_t **current = tree; // Aktualni uzel uzel

    // Hledáme uzel s daným klíčem
    while (NULL != *current  && (*current)->key != key) 
    {
        parent = *current;
        if (key < (*current)->key) 
        {
            current = &((*current)->left);
        } 
        else 
        {
            current = &((*current)->right);
        }
    }

    // Pokud uzel nebyl nalezen
    if (NULL == *current) {
        return;
    }

    bst_node_t *nodeToDelete = *current;

    // Pokud má uzel oba podstromy
    if (NULL != nodeToDelete->left && NULL != nodeToDelete->right) 
    {
        bst_replace_by_rightmost(nodeToDelete, &(nodeToDelete->left));
    } 
    else 
    {
        // Pokud ma uzel pouze jeden podstrom nebo žádný
        bst_node_t *child = (NULL != nodeToDelete->left) ? nodeToDelete->left : nodeToDelete->right;

        // Pokud neni uzel korenem
        if (NULL != parent) 
        {
            if (parent->left == nodeToDelete) 
            {
                parent->left = child;
            } 
            else 
            {
                parent->right = child;
            }
        } 
        else 
        {
            // Pokud je uzel korenem
            *tree = child;
        }

        // Uvolneni uzlu
        free(nodeToDelete);
    }
    
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
	stack_bst_t Stack;
	stack_bst_init(&Stack); // Zasobnik ukazatelu
	while (NULL != *tree || (!stack_bst_empty(&Stack))) 
    {
		if (NULL == *tree){
			if (!stack_bst_empty(&Stack))
			{
				*tree = stack_bst_top(&Stack);
			    stack_bst_pop(&Stack);
			}
		}
        else
		{
			if (NULL != (*tree)->right)
			{
				stack_bst_push(&Stack, (*tree)->right);
			}
			bst_node_t *Aux = *tree;        // Temporary uzel
            *tree = (*tree)->left;
			free(Aux); // Uvolneni aktualniho uzlu
		}
    }
    // Po uvolnění všech uzlů nastavíme původní ukazatel stromu na NULL
    *tree = NULL;
}
/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) 
{
    while (NULL != tree)
    {
		stack_bst_push(to_visit,tree);      // dat uzly na zasobniku 
		bst_add_node_to_items(tree,items);  // zpracovani uzlu na leve diagonale 
		tree = tree->left;                  // posun do leva 
	}
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) 
{
    stack_bst_t Stack;
	// Inicializace zasobniku
	stack_bst_init(&Stack);
    bst_node_t *Actual = NULL;

	// Zavolani funkce bst_lefotmost_preorder
	bst_leftmost_preorder(tree,&Stack,items);       // na zasobnik leva diagonala + zpracovane leve uzly
	while (!stack_bst_empty(&Stack))
	{   // postupne zpracovani ostatnich uzlu
		Actual = stack_bst_top(&Stack);
		stack_bst_pop(&Stack);

		if (NULL != Actual->right)
		{
			bst_leftmost_preorder(Actual->right,&Stack,items);
		}
	}
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while(NULL != tree)
	{   // nezpracovavam pouze je davam do zasobniku
		stack_bst_push(to_visit,tree);
		tree = tree->left;
	}
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) 
{
    stack_bst_t Stack;
	// Inicializace zasobniku
	stack_bst_init(&Stack);

	bst_leftmost_inorder(tree,&Stack);  // na zasobniku leva diagonala  (nezpracovane uzly)
	while (!stack_bst_empty(&Stack))
	{
		bst_node_t *Actual = stack_bst_top(&Stack);
		stack_bst_pop(&Stack);
		bst_add_node_to_items(Actual,items);

		if (NULL != Actual->right)
		{
			bst_leftmost_inorder(Actual->right,&Stack);
		}
		
	}
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) 
{
    while(NULL != tree)
	{
		stack_bst_push(to_visit,tree);
		stack_bool_push(first_visit,true);
		tree = tree->left;
	}
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    bool fromLeft;                  // Indikator zda se vracim zleva
	stack_bst_t Stack;              // zasobnik uzlu
	stack_bool_t FirstVisit;        // zasobnik zda jsme v uzlu poprve

	stack_bool_init(&FirstVisit);   // inicializace
	stack_bst_init(&Stack);         // inicializace
	
    bst_leftmost_postorder(tree,&Stack,&FirstVisit);
	
    while(!stack_bst_empty(&Stack))
	{
		tree = stack_bst_top(&Stack);       // uzel na vrcholu
		fromLeft = stack_bool_top(&FirstVisit); 
		stack_bool_pop(&FirstVisit);
		if (true == fromLeft)   // pokud se vracim zleva
		{
			stack_bool_push(&FirstVisit,false); // musim jit doprava
			bst_leftmost_postorder(tree->right,&Stack,&FirstVisit);
		}
		else
		{   // pokud jdu zprava tak muzu zpracovat uzel 
			stack_bst_pop(&Stack);      
			bst_add_node_to_items(tree,items);			
		}
	}
}

