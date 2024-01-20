/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  // TODO: mozna zkusit pres While, for cyklus bude mit kazdy
  if (NULL != table)
  {
      int ActualPosition;
      for(ActualPosition = 0; ActualPosition < HT_SIZE; ActualPosition++)
      {
          *table[ActualPosition] = NULL;
      }
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if (NULL != table || NULL != key)
  {
      // Najit Index 
      int Index = 0;
      ht_item_t *SearchedItem = NULL;
      Index = get_hash(key);

      SearchedItem = table[Index];

      while(NULL != SearchedItem)
      {
          if (0 == strcmp(key,SearchedItem->key))
          {
            return SearchedItem;
          }
          else
          {
            SearchedItem = SearchedItem->next;
          }
      }
      // Prvek s klicem nebyl nalezen 
      return NULL;
  }
  else
  {
    return NULL;

  }
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {

  if (NULL != table || NULL != key)
  {
      return;
  }
  else
  {

      ht_item_t *IFoundItemWithSameKey; // Searched Item 
      ht_item_t *NewHashItem; // New Item
      IFoundItemWithSameKey = ht_search(table,key);
      if (NULL != IFoundItemWithSameKey)
      {
        IFoundItemWithSameKey->value = value;
      }
      else
      {
        int Index = 0;
        Index = get_hash(key);
        NewHashItem = (ht_item_t*)malloc(sizeof(ht_item_t));
        if (NULL != NewHashItem)
        {
          return;
        }
        else
        {
          NewHashItem->key = strdup(key);
          NewHashItem->value = value;
          NewHashItem->next = (*table)[Index];  // Vlozi prvek na zacatek seznamu
          (*table)[Index] = NewHashItem;
        }
      }
  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if (NULL != table || NULL != key)
  {
    return NULL;
  }
  else{
    ht_item_t *SearchedItem = ht_search(table,key);
    if (NULL == SearchedItem)
    {
      return NULL;
    }
    else
    {
      return &(SearchedItem->value);
    }
  }
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if (NULL != table || NULL != key)
  {
    return;
  }
  else
  {
    int Index;
    Index = get_hash(key);

    ht_item_t *CurrentItem  = (*table)[Index];
    ht_item_t *PreviousItem = NULL;

    while(NULL != CurrentItem)
    {
      // Porovnat klice 
      if (0 == strcmp(key,CurrentItem->key))
      {
        // Pokud je prvek prvni v seznamu
        if (NULL == PreviousItem)
        {
            (*table)[Index] = CurrentItem->next;
        }
        else
        {
            PreviousItem->next = CurrentItem->next;
        }
        // Uvolneni zdroju
        free(CurrentItem->key);
        free(CurrentItem);
      }
      PreviousItem = CurrentItem;
      CurrentItem = CurrentItem->next;
    }
    
  }  
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  int i = 0;
  ht_item_t *CurrentItem = NULL;
  ht_item_t *NextItem = NULL;

  if(NULL == table)
  {
    // Neni co uvolnit
    return;
  }
  else
  {
    // Projdeme kazdy prvek v tabulce
    for (i = 0; i < HT_SIZE; i++)
    {
      CurrentItem = (*table)[i];
      while (NULL != CurrentItem)
      {
        NextItem = CurrentItem->next;
        free(CurrentItem->next);
        free(CurrentItem);

        CurrentItem = NextItem;
      }
    // Nastaveni vsech prvku at ukazuji na NULL
     (*table)[i] = NULL;
    }
  }

}
