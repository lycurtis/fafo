#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_LENGTH 6

/*
Each post office has their own weight limitations
*/
struct package
{
	char* id;
	int weight;
};

typedef struct package package;

struct post_office
{
	int min_weight;
	int max_weight;
	package* packages;
	int packages_count;
};

typedef struct post_office post_office;

struct town
{
	char* name;
	post_office* offices;
	int offices_count;
};

typedef struct town town;



void print_all_packages(town t) {
    printf("%s:\n", t.name); // Town_name:
    
    for(int po = 0; po < t.offices_count; po++){
        printf("\t%d:\n", po); // Post office # (e.g. 0:)
        for(int package = 0; package < t.offices[po].packages_count; package++){
            printf("\t\t%s\n", t.offices[po].packages[package].id); // package id (e.g. a)
        }
    }
}

/*
A GOOD 1st TRY ATTEMPT But not great
*/
// void send_all_acceptable_packages(town* source, int source_office_index, town* target, int target_office_index) {
//     // on every iteration, and you're decrementing it inside the body. The loop shortens as you go and you'll exit before visiting every package. Snapshot the original count into a local before the loop starts, and iterate against that.
//     int original_count = source->offices[source_office_index].packages_count;
//     for(int i = 0; i < original_count; i++){ // walkthrough each pacakge index
//             if(source->offices[source_office_index].packages[i].weight >= target->offices[target_office_index].min_weight
//             && source->offices[source_office_index].packages[i].weight <= target->offices[target_office_index].max_weight){
//                 // transfer the package
//                 target->offices[target_office_index].packages_count += 1; // 1. increase the target PO package count by 1
//                 source->offices[source_office_index].packages_count -= 1; // 2. decrease the source PO package count by 1
                
//                 target->offices[target_office_index].packages = realloc(target->offices[target_office_index].packages, sizeof(package)*target->offices[target_office_index].packages_count); // 3. realloc more memory for target
                
//                 // 4. Move the first package of the source to end of target
//                 int target_package_last_index = target->offices[target_office_index].packages_count - 1;
//                 target->offices[target_office_index].packages[target_package_last_index] = source->offices[source_office_index].packages[i];
                
//                 // 5. Take out the package from source and keep count of how many packages
//                 source->offices[source_office_index].packages[i]
                
                
                
                
//                 source->offices[source_office_index].packages = realloc(source, sizeof(package)*source->offices[source_office_index].packages_count);
                
//             }
//         }
// }

// 2nd Attempt
void send_all_acceptable_packages(town* source, int source_office_index, town* target, int target_office_index) {
    // Step 1. Walk through the source and find the number of packages that are acceptable
    int transfer_count = 0;
    for(int i = 0; i < source->offices[source_office_index].packages_count; i++){
        if(source->offices[source_office_index].packages[i].weight >= target->offices[target_office_index].min_weight
        && source->offices[source_office_index].packages[i].weight <= target->offices[target_office_index].max_weight){
            transfer_count++;
        }
    }
    
    // Step 2. Resize once: grow target by that count 
    if(transfer_count != 0){// Guard the case where the count is 0 (realloc with size 0 is no bueno)
        int updated_target_elements_size = target->offices[target_office_index].packages_count + transfer_count;
        target->offices[target_office_index].packages = realloc(target->offices[target_office_index].packages, sizeof(package)*updated_target_elements_size); // 3. realloc more memory for target
    }

    // Step 3. Walk source again with 2 write positions. 
    // 1 write is for appending accepted packages to target tail, 
    // the other is for compacting rejected packages to the front of source
    int i, keep = 0, add_to_end_index = target->offices[target_office_index].packages_count; // here are my 3 cursors
    // Cursor 1: i is my read index that will be used to walkthrough source array to the end of the original count
    // Cursor 2: keep a write position into source's own array. Starts at 0, advances only when a package is rejected
    // Cursor 3: track valid appendable index of incoming packages from source to target
    int original_count = source->offices[source_office_index].packages_count; // original count snapshot since size will change during the forloop run
    
    for(i = 0; i < original_count; i++){
        if(source->offices[source_office_index].packages[i].weight >= target->offices[target_office_index].min_weight
        && source->offices[source_office_index].packages[i].weight <= target->offices[target_office_index].max_weight){
            target->offices[target_office_index].packages[add_to_end_index] = source->offices[source_office_index].packages[i];
            add_to_end_index++;
        }
        else{
            source->offices[source_office_index].packages[keep] = source->offices[source_office_index].packages[i];
            keep++; // also keeps track of the rejected
        }
    }
    
    // Step 4. Shrink source to the rejected count at the very end, and fix packages_count on both offices once
    source->offices[source_office_index].packages_count = keep;
    // need to make sure to realloc source
    source->offices[source_office_index].packages = realloc(source->offices[source_office_index].packages, sizeof(package)*source->offices[source_office_index].packages_count);
    target->offices[target_office_index].packages_count = add_to_end_index;
    
    
}

town town_with_most_packages(town* towns, int towns_count) {
    town town_with_most_packages;
    int highest_count = 0;
    for(int town = 0; town < towns_count; town++){
        int total_package_count = 0;
        for(int po = 0; po < towns[town].offices_count; po++){
            total_package_count += towns[town].offices[po].packages_count;
        }
        if(total_package_count > highest_count){
            highest_count = total_package_count;
            town_with_most_packages = towns[town];
        }
    }
    
    return town_with_most_packages;
}

town* find_town(town* towns, int towns_count, char* name) {
    town* target = NULL; // need a pointer since return type is a pointer
    for(int town = 0; town < towns_count; town++){
        if(strcmp(towns[town].name, name) == 0){ // if(towns[town].name == name) is WRONG since this compares ptr to ptr
            target = &towns[town]; // need the address since target is a pointer
        }
    }
    return target;
}

int main()
{
	int towns_count;
	scanf("%d", &towns_count);
	town* towns = malloc(sizeof(town)*towns_count);
	for (int i = 0; i < towns_count; i++) {
		towns[i].name = malloc(sizeof(char) * MAX_STRING_LENGTH);
		scanf("%s", towns[i].name);
		scanf("%d", &towns[i].offices_count);
		towns[i].offices = malloc(sizeof(post_office)*towns[i].offices_count);
		for (int j = 0; j < towns[i].offices_count; j++) {
			scanf("%d%d%d", &towns[i].offices[j].packages_count, &towns[i].offices[j].min_weight, &towns[i].offices[j].max_weight);
			towns[i].offices[j].packages = malloc(sizeof(package)*towns[i].offices[j].packages_count);
			for (int k = 0; k < towns[i].offices[j].packages_count; k++) {
				towns[i].offices[j].packages[k].id = malloc(sizeof(char) * MAX_STRING_LENGTH);
				scanf("%s", towns[i].offices[j].packages[k].id);
				scanf("%d", &towns[i].offices[j].packages[k].weight);
			}
		}
	}
	int queries;
	scanf("%d", &queries);
	char town_name[MAX_STRING_LENGTH];
	while (queries--) {
		int type;
		scanf("%d", &type);
		switch (type) {
		case 1:
			scanf("%s", town_name);
			town* t = find_town(towns, towns_count, town_name);
			print_all_packages(*t);
			break;
		case 2:
			scanf("%s", town_name);
			town* source = find_town(towns, towns_count, town_name);
			int source_index;
			scanf("%d", &source_index);
			scanf("%s", town_name);
			town* target = find_town(towns, towns_count, town_name);
			int target_index;
			scanf("%d", &target_index);
			send_all_acceptable_packages(source, source_index, target, target_index);
			break;
		case 3:
			printf("Town with the most number of packages is %s\n", town_with_most_packages(towns, towns_count).name);
			break;
		}
	}
	return 0;
}
