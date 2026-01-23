#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recipe.h"

// 清空输入缓冲区
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 显示主菜单
void display_menu() {
    printf("\n=== 菜品配料管理系统 ===\n");
    printf("1. 添加新菜品\n");
    printf("2. 显示所有菜品\n");
    printf("3. 查找菜品\n");
    printf("4. 查看菜品详情\n");
    printf("5. 删除菜品\n");
    printf("6. 保存到文件\n");
    printf("7. 从文件加载\n");
    printf("8. 统计菜品数量\n");
    printf("9. 添加示例菜品\n");
    printf("0. 退出\n");
    printf("请选择操作 (0-9): ");
}

// 添加新菜品
void add_new_recipe(Recipe** head) {
    char name[100], cuisine[50], difficulty[20];
    int prep_time;
    
    printf("\n--- 添加新菜品 ---\n");
    
    printf("请输入菜名: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // 去除换行符
    
    printf("请输入菜系: ");
    fgets(cuisine, sizeof(cuisine), stdin);
    cuisine[strcspn(cuisine, "\n")] = '\0';
    
    printf("请输入难度(简单/中等/困难): ");
    fgets(difficulty, sizeof(difficulty), stdin);
    difficulty[strcspn(difficulty, "\n")] = '\0';
    
    printf("请输入准备时间(分钟): ");
    scanf("%d", &prep_time);
    clear_input_buffer();
    
    Recipe* recipe = create_recipe(name, cuisine, difficulty, prep_time);
    if (!recipe) {
        printf("创建菜品失败！\n");
        return;
    }
    
    // 添加配料
    printf("\n请输入配料（输入空行结束）:\n");
    int i = 1;
    while (1) {
        char ing_name[50], amount[30];
        
        printf("%d. 配料名: ", i);
        fgets(ing_name, sizeof(ing_name), stdin);
        ing_name[strcspn(ing_name, "\n")] = '\0';
        
        if (strlen(ing_name) == 0) {
            break;
        }
        
        printf("   用量（如200g、1个）: ");
        fgets(amount, sizeof(amount), stdin);
        amount[strcspn(amount, "\n")] = '\0';
        
        add_ingredient(recipe, ing_name, amount);
        i++;
        
        if (recipe->ingredient_count >= MAX_INGREDIENTS) {
            printf("配料数量已达上限！\n");
            break;
        }
    }
    
    // 添加步骤
    printf("\n请输入步骤（输入空行结束）:\n");
    i = 1;
    while (1) {
        char step[256];
        
        printf("%d. ", i);
        fgets(step, sizeof(step), stdin);
        step[strcspn(step, "\n")] = '\0';
        
        if (strlen(step) == 0) {
            break;
        }
        
        add_step(recipe, step);
        i++;
        
        if (recipe->step_count >= MAX_STEPS) {
            printf("步骤数量已达上限！\n");
            break;
        }
    }
    
    add_recipe_to_list(head, recipe);
    printf("菜品 '%s' 添加成功！ID: %d\n", recipe->name, recipe->id);
}

// 查找菜品
void search_recipe(Recipe* head) {
    int choice;
    printf("\n--- 查找菜品 ---\n");
    printf("1. 按ID查找\n");
    printf("2. 按名称查找\n");
    printf("请选择: ");
    scanf("%d", &choice);
    clear_input_buffer();
    
    if (choice == 1) {
        int id;
        printf("请输入菜品ID: ");
        scanf("%d", &id);
        clear_input_buffer();
        
        Recipe* recipe = find_recipe_by_id(head, id);
        if (recipe) {
            display_recipe(recipe);
        } else {
            printf("未找到ID为%d的菜品\n", id);
        }
    } else if (choice == 2) {
        char name[100];
        printf("请输入菜品名称（或部分名称）: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        
        Recipe* recipe = find_recipe_by_name(head, name);
        if (recipe) {
            display_recipe(recipe);
        } else {
            printf("未找到包含'%s'的菜品\n", name);
        }
    } else {
        printf("无效的选择！\n");
    }
}

// 查看菜品详情
void view_recipe_detail(Recipe* head) {
    int id;
    printf("请输入要查看的菜品ID: ");
    scanf("%d", &id);
    clear_input_buffer();
    
    Recipe* recipe = find_recipe_by_id(head, id);
    if (recipe) {
        display_recipe(recipe);
    } else {
        printf("未找到ID为%d的菜品\n", id);
    }
}

// 删除菜品
void delete_recipe(Recipe** head) {
    int id;
    printf("请输入要删除的菜品ID: ");
    scanf("%d", &id);
    clear_input_buffer();
    
    delete_recipe_by_id(head, id);
}

// 添加示例菜品
void add_sample_recipes(Recipe** head) {
    printf("\n添加示例菜品...\n");
    
    // 示例1: 西红柿炒蛋
    Recipe* r1 = create_recipe("西红柿炒蛋", "家常菜", "简单", 15);
    add_ingredient(r1, "西红柿", "2个");
    add_ingredient(r1, "鸡蛋", "3个");
    add_ingredient(r1, "盐", "适量");
    add_ingredient(r1, "糖", "少许");
    add_ingredient(r1, "葱花", "适量");
    add_step(r1, "西红柿洗净切块");
    add_step(r1, "鸡蛋打散，加少许盐搅拌均匀");
    add_step(r1, "热锅凉油，倒入鸡蛋液炒熟盛出");
    add_step(r1, "锅中再加少许油，放入西红柿翻炒");
    add_step(r1, "加入糖和盐，炒至西红柿变软");
    add_step(r1, "倒入炒好的鸡蛋，翻炒均匀");
    add_step(r1, "撒上葱花，出锅");
    add_recipe_to_list(head, r1);
    
    // 示例2: 麻婆豆腐
    Recipe* r2 = create_recipe("麻婆豆腐", "川菜", "中等", 25);
    add_ingredient(r2, "嫩豆腐", "1块");
    add_ingredient(r2, "猪肉末", "100g");
    add_ingredient(r2, "豆瓣酱", "2勺");
    add_ingredient(r2, "花椒", "1勺");
    add_ingredient(r2, "蒜末", "适量");
    add_ingredient(r2, "生抽", "1勺");
    add_ingredient(r2, "水淀粉", "适量");
    add_step(r2, "豆腐切块，放入盐水中浸泡10分钟");
    add_step(r2, "锅中热油，放入肉末炒散");
    add_step(r2, "加入豆瓣酱、蒜末、花椒炒香");
    add_step(r2, "加入适量水，放入豆腐煮5分钟");
    add_step(r2, "加入生抽调味");
    add_step(r2, "最后用水淀粉勾芡，撒上葱花");
    add_recipe_to_list(head, r2);
    
    // 示例3: 青椒肉丝
    Recipe* r3 = create_recipe("青椒肉丝", "家常菜", "简单", 20);
    add_ingredient(r3, "猪里脊", "200g");
    add_ingredient(r3, "青椒", "3个");
    add_ingredient(r3, "姜", "适量");
    add_ingredient(r3, "蒜", "适量");
    add_ingredient(r3, "料酒", "1勺");
    add_ingredient(r3, "生抽", "2勺");
    add_step(r3, "猪里脊切丝，用料酒、生抽腌制10分钟");
    add_step(r3, "青椒切丝，姜蒜切末");
    add_step(r3, "热锅凉油，放入肉丝快速滑炒至变色盛出");
    add_step(r3, "锅中留底油，爆香姜蒜末");
    add_step(r3, "放入青椒丝翻炒至断生");
    add_step(r3, "倒入肉丝，加适量盐和生抽调味");
    add_step(r3, "快速翻炒均匀即可出锅");
    add_recipe_to_list(head, r3);
    
    printf("成功添加3个示例菜品！\n");
}

// 主函数
int main() {
    Recipe* head = NULL;
    int choice;
    
    printf("欢迎使用菜品配料管理系统！\n");
    
    // 尝试从文件加载现有数据
    head = load_recipes_from_file(FILENAME);
    
    do {
        display_menu();
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch (choice) {
            case 1:
                add_new_recipe(&head);
                break;
            case 2:
                display_all_recipes(head);
                break;
            case 3:
                search_recipe(head);
                break;
            case 4:
                view_recipe_detail(head);
                break;
            case 5:
                delete_recipe(&head);
                break;
            case 6:
                save_recipes_to_file(head, FILENAME);
                break;
            case 7:
                {
                    Recipe* new_head = load_recipes_from_file(FILENAME);
                    if (new_head) {
                        // 释放旧内存
                        Recipe* current = head;
                        while (current) {
                            Recipe* next = current->next;
                            free(current);
                            current = next;
                        }
                        head = new_head;
                    }
                }
                break;
            case 8:
                printf("\n当前共有 %d 个菜品\n", count_recipes(head));
                break;
            case 9:
                add_sample_recipes(&head);
                break;
            case 0:
                printf("\n感谢使用，再见！\n");
                // 退出前自动保存
                if (count_recipes(head) > 0) {
                    char save;
                    printf("是否保存数据到文件？(y/n): ");
                    scanf("%c", &save);
                    clear_input_buffer();
                    if (save == 'y' || save == 'Y') {
                        save_recipes_to_file(head, FILENAME);
                    }
                }
                break;
            default:
                printf("无效的选择，请重新输入！\n");
        }
    } while (choice != 0);
    
    // 清理内存
    Recipe* current = head;
    while (current) {
        Recipe* next = current->next;
        free(current);
        current = next;
    }
    
    return 0;
}
