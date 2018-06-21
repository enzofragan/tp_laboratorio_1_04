#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/ArrayList.h"

// funciones privadas
int resizeUp(ArrayList* this);
int expand(ArrayList* this,int index);
int contract(ArrayList* this,int index);

#define AL_INCREMENT      10
#define AL_INITIAL_VALUE  10
//___________________

/** \brief Allocate a new arrayList with AL_INITIAL_VALUE elements.
 * \param void
 * \return ArrayList* Return (NULL) if Error [if can't allocate memory]
 *                  - (pointer to new arrayList) if ok
 */
ArrayList* al_newArrayList(void)
{
    ArrayList* this;
    ArrayList* returnAux = NULL;
    void* pElements;///aux
    this = (ArrayList *)malloc(sizeof(ArrayList));

    if(this != NULL)
    {
        pElements = malloc(sizeof(void *)*AL_INITIAL_VALUE );///malloc de aux (malloc se usa para gregar espacio en memoria)
        if(pElements != NULL)
        {
            this->size=0;
            this->pElements=pElements;///primer pElements no aux y segundo pElements aux
            this->reservedSize=AL_INITIAL_VALUE;
            this->add=al_add;
            this->len=al_len;
            this->set=al_set;
            this->remove=al_remove;
            this->clear=al_clear;
            this->clone=al_clone;
            this->get=al_get;
            this->contains=al_contains;
            this->push=al_push;
            this->indexOf=al_indexOf;
            this->isEmpty=al_isEmpty;
            this->pop=al_pop;
            this->subList=al_subList;
            this->containsAll=al_containsAll;
            this->deleteArrayList = al_deleteArrayList;
            this->sort = al_sort;
            returnAux = this;
        }
        else
        {
            free(this);
        }
    }

    return returnAux;
}



/** \brief  Add an element to arrayList and if is
 *          nessesary resize the array
 * \param pList ArrayList* Pointer to arrayList
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [pList or pElement are NULL pointer] - (0) if Ok
 *
 */
int al_add(ArrayList* this, void* pElement)
{
    int returnAux = -1;
    void** aux;
    int flag=0;
    if(this!=NULL && pElement!=NULL)///verifica que el arrayLIst y el elemento a agregar no este vacio
    {
        if(this->size == this->reservedSize)///si el tamaño del arrayList es igual al espacio reservado
        {
            aux=(void**) realloc(this->pElements, sizeof(void*)*(this->reservedSize + AL_INCREMENT));
            ///el aux(element) se hace un realloc(realloc es usado para aumentar el espacio en memoria por el tamaño a aumentar) aumentando el espacio reservado
            if(aux!=NULL)///como se inicio el aux hay que verificar si no es nulo
            {
                this->pElements=aux;///a loselementos si le asigna lo que se realizo en aux
                this->reservedSize=this->reservedSize + AL_INCREMENT;///aumenta el espacio reservado
            }
            else
            {
                flag=1;
            }
        }

        if(flag==0)///la bandera(flag) funciona para saber si se pudo aumentar el tamaño
        {
            *(this->pElements+this->size)=pElement;///el elemento siguiente al ultimo agregado se le asigna el elemento deseado
            this->size++;///aumenta en 1 el tamaño de los agregados
            returnAux = 0;
        }
    }

    return returnAux;
}

/** \brief  Delete arrayList
 * \param pList ArrayList* Pointer to arrayList
 * \return int Return (-1) if Error [pList is NULL pointer] - (0) if Ok
 *
 */
int al_deleteArrayList(ArrayList* this)
{
    int returnAux = -1;
    if(this!=NULL)
    {
        free(this);///free es usado para eliminar el array completo de todas las estructuras
        returnAux=0;
    }

    return returnAux;
}

/** \brief  Delete arrayList
 * \param pList ArrayList* Pointer to arrayList
 * \return int Return length of array or (-1) if Error [pList is NULL pointer]
 *
 */
int al_len(ArrayList* this)
{
    int returnAux = -1;
    if(this!=NULL)
    {
        returnAux = this->size;///devuelve el tamaño de todos los agregados
    }

    return returnAux;
}


/** \brief  Get an element by index
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \return void* Return (NULL) if Error [pList is NULL pointer or invalid index] - (Pointer to element) if Ok
 *
 */
void* al_get(ArrayList* this, int index)
{
    void* returnAux = NULL;
   /// if(this!=NULL && *(this->pElements+index)!=NULL && index<al_len(this))
        if(this!=NULL && *(this->pElements+index)!=NULL && index<this->len(this))
    ///si la lista no es nula y elemento de esa lista en la posicion deseada(index) no es nulo Y la posicion es menor al tamaño de la lista
    {
        returnAux=*(this->pElements+index);///retorna el elemento de esa posicion
    }

    return returnAux;
}


/** \brief  Find if pList contains at least one element pElement
 * \param pList ArrayList* Pointer to arrayList
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [pList or pElement are NULL pointer]
 *                  - ( 0) if Ok but not found a element
 *                  - ( 1) if this list contains at least one element pElement
 *
 */
int al_contains(ArrayList* this, void* pElement)
{
    int returnAux = -1;
    int i;
    if(this!=NULL && pElement!=NULL)
    {
        returnAux=0;
        for(i=0;i<this->len(this);i++)
        {
            if(*(this->pElements+i)==pElement)
            {
                returnAux=1;
            }
        }
    }
    return returnAux;
}


/** \brief  Set a element in pList at index position
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [pList or pElement are NULL pointer or invalid index]
 *                  - ( 0) if Ok
 *
 */
int al_set(ArrayList* this, int index,void* pElement)
{
    int returnAux = -1;
    int espacio;

    if(this!=NULL && pElement!=NULL && index>=0)
    {
        espacio=this->len(this);

        if(index==espacio)
        {
            al_add(this,pElement);
               returnAux=0;
        }
        else if(index<espacio)
        {
            *(this->pElements+index)=pElement;
            returnAux=0;
        }

    }
    return returnAux;
}


/** \brief  Remove an element by index
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \return int Return (-1) if Error [pList is NULL pointer or invalid index]
 *                  - ( 0) if Ok
 */
int al_remove(ArrayList* this,int index)
{
    int returnAux = -1;
    int remo;
    if(this!=NULL)
    {
        if(index<this->len(this) && index>=0)
        {
            remo=contract(this,index);
            if(remo==0)
            {
                returnAux=0;
            }
        }
    }

    return returnAux;
}



/** \brief Removes all of the elements from this list
 * \param pList ArrayList* Pointer to arrayList
 * \return int Return (-1) if Error [pList is NULL pointer]
 *                  - ( 0) if Ok
 */
int al_clear(ArrayList* this)
{
    int returnAux = -1;
    if(this!=NULL && this->pElements!=NULL)
    {
        this->size=0;
        this->reservedSize=AL_INITIAL_VALUE;
        this->pElements=(void**) malloc(sizeof(void*)*AL_INITIAL_VALUE);
        returnAux=0;
    }

    return returnAux;
}



/** \brief Returns an array containing all of the elements in this list in proper sequence
 * \param pList ArrayList* Pointer to arrayList
 * \return ArrayList* Return  (NULL) if Error [pList is NULL pointer]
 *                          - (New array) if Ok
 */
ArrayList* al_clone(ArrayList* this)
{
    ArrayList* returnAux = NULL;
    int i;
    void* aux=NULL;

    if(this!=NULL)
    {
        returnAux=al_newArrayList();
        if(returnAux!=NULL)
        {
            for(i=0;i<this->len(this);i++)
            {
                if(resizeUp(returnAux)==0)
                {
                    aux=this->get(this,i);
                    returnAux->add(returnAux,aux);
                }
            }
        }
    }

    return returnAux;
}




/** \brief Inserts the element at the specified position
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [pList or pElement are NULL pointer or invalid index]
 *                  - ( 0) if Ok
 */
int al_push(ArrayList* this, int index, void* pElement)
{
    int returnAux = -1;

    if(this!=NULL && pElement!=NULL)
    {
        if(al_set(this,index,pElement)==0)
        {
            returnAux=0;
        }
    }
    return returnAux;
}



/** \brief Returns the index of the first occurrence of the specified element
 * \param pList ArrayList* Pointer to arrayList
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [pList or pElement are NULL pointer] - (index to element) if Ok
 */
int al_indexOf(ArrayList* this, void* pElement)
{
    int returnAux = -1;
    int i;

    if(this!=NULL && pElement!=NULL)
    {
        for(i=0;i<this->len(this);i++)
        {
            if(*(this->pElements+i)==pElement)
            {
                returnAux=i;
            }
        }
    }

    return returnAux;
}



/** \brief Returns true if this list contains no elements.
 * \param pList ArrayList* Pointer to arrayList
 * \return int Return (-1) if Error [pList is NULL pointer] - (0) if Not Empty - (1) if is Empty
 */
int al_isEmpty(ArrayList* this)
{
    int returnAux = -1;
    int lista;

    if(this!=NULL)
    {
        lista=this->len(this);

        if(lista==0)
        {
            returnAux=1;
        }
        else
        {
            returnAux=0;
        }
    }

    return returnAux;
}




/** \brief Remove the item at the given position in the list, and return it.
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \return int Return (NULL) if Error [pList is NULL pointer or invalid index]
 *                  - ( element pointer) if Ok
 */
void* al_pop(ArrayList* this,int index)
{
    void* returnAux = NULL;

    if(this!=NULL && this->pElements!=NULL && index<this->len(this) && index>=0)
    {
        returnAux=*(this->pElements + index);
        contract(this,index);
    }

    return returnAux;
}


/** \brief Returns a new arrayList with a portion of pList between the specified
 *         fromIndex, inclusive, and toIndex, exclusive.
 * \param pList ArrayList* Pointer to arrayList
 * \param from int Initial index of the element (inclusive)
 * \param to int Final index of the element (exclusive)
 * \return int Return (NULL) if Error [pList is NULL pointer or invalid 'from' or invalid 'to']
 *                  - ( pointer to new array) if Ok
 */
ArrayList* al_subList(ArrayList* this,int from,int to)
{
    void* returnAux = NULL;
    int i;
    ArrayList* auxElement = NULL;

    if(this!=NULL && from>=0 && from<=to && to<=this->len(this))
    {
        auxElement=al_newArrayList();
        if(auxElement!=NULL)
        {
            for(i=from;i<to;i++)
            {
                if(resizeUp(auxElement)==0)
                {
                    returnAux=this->get(this,i);
                    auxElement->add(auxElement,returnAux);
                }
            }
        }
    }

    return auxElement ;
}





/** \brief Returns true if pList list contains all of the elements of pList2
 * \param pList ArrayList* Pointer to arrayList
 * \param pList2 ArrayList* Pointer to arrayList
 * \return int Return (-1) if Error [pList or pList2 are NULL pointer]
 *                  - (0) if Not contains All - (1) if is contains All
 */
int al_containsAll(ArrayList* this,ArrayList* this2)
{
    int returnAux = -1;
    int i;
    int j;
    int contElement=0;

    if(this!=NULL && this2!=NULL)
    {
        for(i=0;i<this->len(this);i++)
        {
            for(j=0;j<this2->len(this2);j++)
            {
                if(*(this->pElements+i)==*(this2->pElements+j))
                {
                    contElement++;
                    break;
                }
            }
        }

        if(contElement==al_len(this2))
        {
            returnAux=1;
        }
        else
        {
            returnAux=0;
        }
    }

    return returnAux;
}

/** \brief Sorts objects of list, use compare pFunc
 * \param pList ArrayList* Pointer to arrayList
 * \param pFunc (*pFunc) Pointer to fuction to compare elements of arrayList
 * \param order int  [1] indicate UP - [0] indicate DOWN
 * \return int Return (-1) if Error [pList or pFunc are NULL pointer]
 *                  - (0) if ok
 */
int al_sort(ArrayList* this, int (*pFunc)(void* ,void*), int order)
{
    int returnAux = -1;
    int i;
    int j;
    void* aux=NULL;
    void* pEA;
    void* pEB;

    if(this!=NULL && pFunc!=NULL && (order==0 || order==1))
    {
        for(i=0;i<this->len(this)-1;i++)
        {
            for(j=i+1;j<this->len(this);j++)
            {
                pEA=this->get(this,i);
                pEB=this->get(this,j);
                if(order==1)
                {
                    if(pFunc(pEA,pEB)==1)
                    {
                        aux=*(this->pElements+i);
                        *(this->pElements+i)=*(this->pElements+j);
                        *(this->pElements+j)=aux;
                    }
                }
                else
                {
                    if(pFunc(pEA,pEB)==-1)
                    {
                        aux=*(this->pElements+i);
                        *(this->pElements+i)=*(this->pElements+j);
                        *(this->pElements+j)=aux;
                    }
                }
            }
        }
        returnAux=0;
    }

    return returnAux;
}


/** \brief Increment the number of elements in pList in AL_INCREMENT elements.
 * \param pList ArrayList* Pointer to arrayList
 * \return int Return (-1) if Error [pList is NULL pointer or if can't allocate memory]
 *                  - (0) if ok
 */
int resizeUp(ArrayList* this)
{
    int returnAux = 0;
    void* aux=NULL;
    if(this->size==this->reservedSize)
    {
        aux=(void*)realloc(this->pElements,sizeof(void*)*(this->reservedSize+AL_INCREMENT));

        if(aux!=NULL)
        {
            this->reservedSize+=AL_INCREMENT;
            this->pElements=aux;
        }
        else
        {
            returnAux=-1;
        }
    }


    return returnAux;

}

/** \brief  Expand an array list
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \return int Return (-1) if Error [pList is NULL pointer or invalid index]
 *                  - ( 0) if Ok
 */
int expand(ArrayList* this,int index)///desde un indice expande el tamaño
{
    int returnAux = -1;
    int i;

    if(this!=NULL)
    {
        this->size+=1;
        resizeUp(this);

        for(i=this->len(this);i<index;i--)
        {
            *(this->pElements+i)=*(this->pElements+i-1);
        }
        returnAux=0;
    }

    return returnAux;
}

/** \brief  Contract an array list
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \return int Return (-1) if Error [pList is NULL pointer or invalid index]
 *                  - ( 0) if Ok
 */
int contract(ArrayList* this,int index)
{
    int returnAux = -1;
    int i;

    if(this!=NULL && this->pElements!=NULL)
    {
        for(i=index;i<this->len(this);i++)
        {
            *(this->pElements+i)=*(this->pElements+i+1);
        }
        this->size-=1;
        returnAux=0;
    }

    return returnAux;
}
