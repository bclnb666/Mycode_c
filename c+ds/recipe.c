#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recipe.h"

// 全局变量，用于生成ID
static int next_id = 1;

// 创建新菜品
Recipe* create_recipe(const char* name, const char* cuisine, 
                      const char* difficulty, int prep_time) {
    Recipe* recipe = (Recipe*)malloc(sizeof(Recipe));
    if (!recipe) {
        printf("内存分配失败！\n");
        return NULL;
    }
    
    recipe->id = next_id++;
    strncpy(recipe->name, name, sizeof(recipe->name) - 1);
    strncpy(recipe->cuisine, cuisine, sizeof(recipe->cuisine) - 1);
    strncpy(recipe->difficulty, difficulty, sizeof(recipe->difficulty) - 1);
    recipe->prep_time = prep_time;
    recipe->ingredient_count = 0;
    recipe->step_count = 0;
    recipe->next = NULL;
    
    return recipe;
}

// 添加配料
void add_ingredient(Recipe* recipe, const char* name, const char* amount) {
    if (!recipe || recipe->ingredient_count >= MAX_INGREDIENTS) {
        printf("无法添加更多配料！\n");
        return;
    }
    
    int idx = recipe->ingredient_count;
    strncpy(recipe->ingredients[idx].name, name, 
            sizeof(recipe->ingredients[idx].name) - 1);
    strncpy(recipe->ingredients[idx].amount, amount, 
            sizeof(recipe->ingredients[idx].amount) - 1);
    recipe->ingredient_count++;
}

// 添加步骤
void add_step(Recipe* recipe, const char* step) {
    if (!recipe || recipe->step_count >= MAX_STEPS) {
        printf("无法添加更多步骤！\n");
        return;
    }
    
    int idx = recipe->step_count;
    strncpy(recipe->steps[idx], step, sizeof(recipe->steps[idx]) - 1);
    recipe->step_count++;
}

// 显示菜品详细信息
void display_recipe(Recipe* recipe) {
    if (!recipe) {
        printf("菜品不存在！\n");
        return;
    }
    
    printf("\n========== %s ==========\n", recipe->name);
    printf("ID: %d\n", recipe->id);
    printf("菜系: %s\n", recipe->cuisine);
    printf("难度: %s\n", recipe->difficulty);
    printf("准备时间: %d分钟\n", recipe->prep_time);
    
    printf("\n配料:\n");
    for (int i = 0; i < recipe->ingredient_count; i++) {
        printf("  %d. %s - %s\n", i + 1, 
               recipe->ingredients[i].name, 
               recipe->ingredients[i].amount);
    }
    
    printf("\n步骤:\n");
    for (int i = 0; i < recipe->step_count; i++) {
        printf("  %d. %s\n", i + 1, recipe->steps[i]);
    }
    printf("=============================\n\n");
}

// 显示菜品简略信息
void display_recipe_simple(Recipe* recipe) {
    if (!recipe) return;
    printf("[%d] %s (%s, %s, %d分钟)\n", 
           recipe->id, recipe->name, recipe->cuisine, 
           recipe->difficulty, recipe->prep_time);
}

// 释放菜品内存
void free_recipe(Recipe* recipe) {
    if (recipe) {
        free(recipe);
    }
}

// ========== 链表操作 ==========

// 添加菜品到链表
void add_recipe_to_list(Recipe** head, Recipe* new_recipe) {
    if (!new_recipe) return;
    
    if (*head == NULL) {
        *head = new_recipe;
    } else {
        Recipe* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_recipe;
    }
}

// 显示所有菜品
void display_all_recipes(Recipe* head) {
    if (!head) {
        printf("还没有菜品，请先添加！\n");
        return;
    }
    
    printf("\n=== 所有菜品 ===\n");
    Recipe* current = head;
    while (current != NULL) {
        display_recipe_simple(current);
        current = current->next;
    }
    printf("================\n\n");
}

// 根据ID查找菜品
Recipe* find_recipe_by_id(Recipe* head, int id) {
    Recipe* current = head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 根据名称查找菜品
Recipe* find_recipe_by_name(Recipe* head, const char* name) {
    Recipe* current = head;
    while (current != NULL) {
        if (strstr(current->name, name) != NULL) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 根据ID删除菜品
int delete_recipe_by_id(Recipe** head, int id) {
    if (!*head) return 0;
    
    Recipe* current = *head;
    Recipe* prev = NULL;
    
    // 如果是头节点
    if (current != NULL && current->id == id) {
        *head = current->next;
        free_recipe(current);
        printf("菜品删除成功！\n");
        return 1;
    }
    
    // 查找要删除的节点
    while (current != NULL && current->id != id) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) {
        printf("未找到ID为%d的菜品\n", id);
        return 0;
    }
    
    prev->next = current->next;
    free_recipe(current);
    printf("菜品删除成功！\n");
    return 1;
}

// 统计菜品数量
int count_recipes(Recipe* head) {
    int count = 0;
    Recipe* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// ========== 文件操作 ==========

// 保存菜品到文件
int save_recipes_to_file(Recipe* head, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("无法打开文件 %s 进行保存！\n", filename);
        return 0;
    }
    
    // 保存next_id
    fwrite(&next_id, sizeof(int), 1, file);
    
    // 保存菜品数量
    int count = count_recipes(head);
    fwrite(&count, sizeof(int), 1, file);
    
    // 保存每个菜品
    Recipe* current = head;
    while (current != NULL) {
        fwrite(current, sizeof(Recipe), 1, file);
        current = current->next;
    }
    
    fclose(file);
    printf("成功保存 %d 个菜品到 %s\n", count, filename);
    return 1;
}

// 从文件加载菜品
Recipe* load_recipes_from_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("无法打开文件 %s，将创建新文件\n", filename);
        return NULL;
    }
    
    // 读取next_id
    fread(&next_id, sizeof(int), 1, file);
    
    // 读取菜品数量
    int count;
    fread(&count, sizeof(int), 1, file);
    
    Recipe* head = NULL;
    for (int i = 0; i < count; i++) {
        Recipe* recipe = (Recipe*)malloc(sizeof(Recipe));
        if (!recipe) {
            printf("内存分配失败！\n");
            break;
        }
        
        fread(recipe, sizeof(Recipe), 1, file);
        recipe->next = NULL;
        
        add_recipe_to_list(&head, recipe);
    }
    
    fclose(file);
    printf("成功从 %s 加载 %d 个菜品\n", filename, count);
    return head;
}
