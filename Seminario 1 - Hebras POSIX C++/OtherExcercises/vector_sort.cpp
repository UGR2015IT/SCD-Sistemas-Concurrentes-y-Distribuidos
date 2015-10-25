//
// Created by dave95 on 24/10/15.
//

#include <pthread.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

const unsigned long dim_vect=100, n_threads=10;

unsigned vector_to_sort[dim_vect],vector_of_the_minimums[n_threads],counter=0;

sem_t sem_vector_sorted[n_threads], sem_can_sort[n_threads], mutex;

void fill_vector(){
    cout << "Filling the vector ... ";
    for (unsigned i=0;i<dim_vect;i++){
        vector_to_sort[i]=dim_vect-i;
    }
}

bool check_sort(){
    for (unsigned long i=0; i<(dim_vect-1);i++){
        if (vector_to_sort[i]>vector_to_sort[i+1])
            return false;
    }
    return true;
}

unsigned* check_for_min(unsigned new_vector[], unsigned dim){
    unsigned min = new_vector[0], min_index=0;
    unsigned long ext_i, iter;
    for (ext_i=0;ext_i<dim_vect/n_threads;ext_i++){
        for (iter=ext_i+1;iter<dim_vect/n_threads;iter++){
            if (new_vector[ext_i]<min) {
                min = new_vector[ext_i];
                min_index=ext_i;
            }
        }
    }
    unsigned checkout[2]={min, min_index};
    return checkout;
}

void* minimum_sort(void* p){
    // Wait for all the threads to have generated a minimum
    for (unsigned y=0; y<n_threads;y++){
        sem_wait(&sem_can_sort[y]);
    }
    // Checks for his own minimum
    unsigned my_minimum[2];
    my_minimum[0]=check_for_min(vector_of_the_minimums, n_threads)[0];
    my_minimum[1]=check_for_min(vector_of_the_minimums, n_threads)[1];
    // Writes back the minimum in his vector, increases the counter in the original vector
    vector_to_sort[counter]=my_minimum[0];
    counter++;
    // Unlocks the thread of the found minimum
    sem_post(&sem_vector_sorted[my_minimum[1]]);

    return NULL;
}

void* sorting(void* id_thread){
    while (!check_sort()){
        unsigned small_vector[dim_vect/n_threads],k;
        unsigned long id = (unsigned long) id_thread;
        unsigned j;

        // Filling the small vector associated to the thread[id_thread]
        for (j=id*(dim_vect/n_threads);j<(id+1)*(dim_vect/n_threads);j++){
            small_vector[k]=vector_to_sort[j];
            k++;
        }
        
        // Wait for his place in the vector of the minimums to be freed
        sem_wait(&sem_vector_sorted[id]);

        // Checking for the minimum in the vector and putting it in the vector_of_the_minimums
        unsigned result[2];
        result[0]=check_for_min(small_vector,dim_vect/n_threads)[0];
        result[1]=check_for_min(small_vector,dim_vect/n_threads)[1];
        vector_of_the_minimums[id]=result[0];
        small_vector[result[1]]=n_threads+1;

        sem_post (&sem_can_sort[id]);

        return NULL;

    }
}

void print_vector(){
    cout << "I finished sorting the vector. Here it is:"<<endl;
    for (unsigned i=0;i<dim_vect;i++)
        cout << vector_to_sort[i] << "/t";
}

int main(){

    fill_vector();

    for (unsigned i=0; i<n_threads; i++){
        sem_init(&sem_vector_sorted[i], 0, 1);
        sem_init(&sem_can_sort[i], 0, 0);
    }

    pthread_t vector_sorter[n_threads];
    pthread_t thread_for_minimum;
    for (unsigned long i=0;i<n_threads;i++){
        pthread_create(&vector_sorter[i], NULL, sorting, (void*)i);
    }
    pthread_create(&thread_for_minimum, NULL, minimum_sort, NULL);
    for (unsigned i=0;i<n_threads;i++){
        pthread_join(vector_sorter[i], NULL);
    }
    pthread_join(thread_for_minimum, NULL);
    pthread_exit(NULL);

    print_vector();
    return 0;

}