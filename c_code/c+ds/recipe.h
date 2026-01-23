#ifndef RECIPE_H
#define RECIPE_H

#define MAX_INGREDIENTS 20
#define MAX_STEPS 10
#define MAX_RECIPES 100
#define FILENAME "recipes.dat"

// 配料结构
typedef struct Ingredient {
    char name[50];      // 配料名
    char amount[30];    // 用量（如"200g"、"1个"）
} Ingredient;

// 菜品结构
typedef struct Recipe {
    int id;                    // 菜品ID
    char name[100];           // 菜名
    char cuisine[50];         // 菜系
    char difficulty[20];      // 难度（简单/中等/困难）
    int prep_time;            // 准备时间（分钟）
    
    Ingredient ingredients[MAX_INGREDIENTS];  // 配料数组
    int ingredient_count;
    
    char steps[MAX_STEPS][256];  // 步骤数组
    int step_count;
    
    struct Recipe* next;      // 链表指针
} Recipe;

// 函数声明
Recipe* create_recipe(const char* name, const char* cuisine, 
                      const char* difficulty, int prep_time);
void add_ingredient(Recipe* recipe, const char* name, const char* amount);
void add_step(Recipe* recipe, const char* step);
void display_recipe(Recipe* recipe);
void display_recipe_simple(Recipe* recipe);
void free_recipe(Recipe* recipe);

// 链表操作
void add_recipe_to_list(Recipe** head, Recipe* new_recipe);
void display_all_recipes(Recipe* head);
Recipe* find_recipe_by_id(Recipe* head, int id);
Recipe* find_recipe_by_name(Recipe* head, const char* name);
int delete_recipe_by_id(Recipe** head, int id);
int count_recipes(Recipe* head);

// 文件操作
int save_recipes_to_file(Recipe* head, const char* filename);
Recipe* load_recipes_from_file(const char* filename);

#endif
