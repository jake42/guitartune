#include <Elementary.h>

#define MAX_STATES 17

Evas_Object *root, *bg, *layout,
            *list,  *state_icon, *state_end;

Evas_Object *state_btn[MAX_STATES];

static void state_set(void *data, Evas_Object *obj, void *event_info){
    char * cmd = (char *)data;
    asprintf(&cmd,"alsactl -f /usr/share/openmoko/scenarios/%s restore", (const char*) data );
    system(cmd);
}
static void app_signal_exit(void *data, Evas_Object *obj, void *event_info){
    elm_shutdown();
    exit(0);
}

int elm_main(int argc, char **argv){
    
     root = elm_win_add(NULL, "Sound State Chooser", ELM_WIN_BASIC);
    elm_win_title_set(root, "Sound State Chooser");
    elm_win_borderless_set(root, 1);
    elm_win_autodel_set(root, 1);

    bg = elm_bg_add(root);
    evas_object_size_hint_weight_set(bg, 1.0, 1.0);
    elm_win_resize_object_add(root, bg);

    layout=elm_box_add(root);
    evas_object_size_hint_weight_set(layout, 1.0, 1.0);
    elm_win_resize_object_add(root, layout);

    char * li= argv[argc-1];
    for (int x = argc-1; x>0;){
        state_btn[x] = elm_button_add(root);
        elm_button_label_set(state_btn[x], argv[x]);
        elm_box_pack_start(layout,state_btn[x]);
        evas_object_show(state_btn[x]);
        evas_object_smart_callback_add(state_btn[x], "clicked", state_set, argv[x]);
        x--;
        li = argv[x];
        printf("%d: %s \n" , x, li);
    }

    evas_object_show(layout);
    evas_object_show(bg);
    evas_object_show(root);

    evas_object_smart_callback_add(root, "delete-request", app_signal_exit, NULL);

    elm_run();
    return 0;               
}                               
ELM_MAIN()
