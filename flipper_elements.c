#include <furi.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/text_box.h>
#include <gui/modules/dialog_ex.h> 
#include <gui/view.h>
#include <stdio.h>
#include <string.h>

#define VIEW_SPLASH 0
#define VIEW_GRID 1
#define VIEW_DETAILS 2
#define VIEW_ABOUT 3 
#define VIEW_EXIT_PROMPT 4 
#define VIEW_EXIT_SPLASH 5

#define EVENT_START_APP 101 
#define EVENT_SHOW_DETAILS 102
#define EVENT_SHOW_ABOUT 103
#define EVENT_FINAL_CLOSE 104

#define E(x, y, num, p, g, sym, n, m, mp, bp, c, e) \
    { x, y, num, p, g, sym, n, m, mp, bp, c, e }

typedef struct {
    uint8_t col, row, z, period, group;
    const char* symbol; const char* name; const char* mass; const char* melt; 
    const char* boil; const char* cat; const char* e_conf;       
} Element;

const Element DB[] = {
E(0,0,1,1,1,"H","Hydrogen","1.008","13.99","20.27","nonmetal gas","1s1"),
E(17,0,2,1,18,"He","Helium","4.0026","0.95","4.22","noble gas","1s2"),
E(0,1,3,2,1,"Li","Lithium","6.94","453.6","1603","alkali metal","[He] 2s1"),
E(1,1,4,2,2,"Be","Beryllium","9.012","1560","2742","alkaline earth","[He] 2s2"),
E(12,1,5,2,13,"B","Boron","10.81","2349","4200","metalloid","[He] 2s2 2p1"),
E(13,1,6,2,14,"C","Carbon","12.01","N/A","N/A","poly nonmetal","[He] 2s2 2p2"),
E(14,1,7,2,15,"N","Nitrogen","14.00","63.15","77.35","dia nonmetal","[He] 2s2 2p3"),
E(15,1,8,2,16,"O","Oxygen","15.99","54.36","90.18","dia nonmetal","[He] 2s2 2p4"),
E(16,1,9,2,17,"F","Fluorine","18.99","53.48","85.03","dia nonmetal","[He] 2s2 2p5"),
E(17,1,10,2,18,"Ne","Neon","20.18","24.56","27.1","noble gas","[He] 2s2 2p6"),
E(0,2,11,3,1,"Na","Sodium","22.98","370.9","1156","alkali metal","[Ne] 3s1"),
E(1,2,12,3,2,"Mg","Magnesium","24.30","923","1363","alkaline earth","[Ne] 3s2"),
E(12,2,13,3,13,"Al","Aluminium","26.98","933.4","2743","post-transition","[Ne] 3s2 3p1"),
E(13,2,14,3,14,"Si","Silicon","28.08","1687","3538","metalloid","[Ne] 3s2 3p2"),
E(14,2,15,3,15,"P","Phosphorus","30.97","N/A","N/A","poly nonmetal","[Ne] 3s2 3p3"),
E(15,2,16,3,16,"S","Sulfur","32.06","388.3","717.8","poly nonmetal","[Ne] 3s2 3p4"),
E(16,2,17,3,17,"Cl","Chlorine","35.45","171.6","239.1","dia nonmetal","[Ne] 3s2 3p5"),
E(17,2,18,3,18,"Ar","Argon","39.94","83.81","87.3","noble gas","[Ne] 3s2 3p6"),
E(0,3,19,4,1,"K","Potassium","39.09","336.7","1032","alkali metal","[Ar] 4s1"),
E(1,3,20,4,2,"Ca","Calcium","40.07","1115","1757","alkaline earth","[Ar] 4s2"),
E(2,3,21,4,3,"Sc","Scandium","44.95","1814","3109","transition metal","[Ar] 3d1 4s2"),
E(3,3,22,4,4,"Ti","Titanium","47.86","1941","3560","transition metal","[Ar] 3d2 4s2"),
E(4,3,23,4,5,"V","Vanadium","50.94","2183","3680","transition metal","[Ar] 3d3 4s2"),
E(5,3,24,4,6,"Cr","Chromium","51.99","2180","2944","transition metal","[Ar] 3d5 4s1"),
E(6,3,25,4,7,"Mn","Manganese","54.93","1519","2334","transition metal","[Ar] 3d5 4s2"),
E(7,3,26,4,8,"Fe","Iron","55.84","1811","3134","transition metal","[Ar] 3d6 4s2"),
E(8,3,27,4,9,"Co","Cobalt","58.93","1768","3200","transition metal","[Ar] 3d7 4s2"),
E(9,3,28,4,10,"Ni","Nickel","58.69","1728","3003","transition metal","[Ar] 3d8 4s2"),
E(10,3,29,4,11,"Cu","Copper","63.54","1357","2835","transition metal","[Ar] 3d10 4s1"),
E(11,3,30,4,12,"Zn","Zinc","65.38","692.6","1180","transition metal","[Ar] 3d10 4s2"),
E(12,3,31,4,13,"Ga","Gallium","69.72","302.9","2673","post-transition","[Ar] 3d10 4s2 4p1"),
E(13,3,32,4,14,"Ge","Germanium","72.63","1211","3106","metalloid","[Ar] 3d10 4s2 4p2"),
E(14,3,33,4,15,"As","Arsenic","74.92","N/A","N/A","metalloid","[Ar] 3d10 4s2 4p3"),
E(15,3,34,4,16,"Se","Selenium","78.97","494","958","poly nonmetal","[Ar] 3d10 4s2 4p4"),
E(16,3,35,4,17,"Br","Bromine","79.90","265.8","332","dia nonmetal","[Ar] 3d10 4s2 4p5"),
E(17,3,36,4,18,"Kr","Krypton","83.79","115.7","119.9","noble gas","[Ar] 3d10 4s2 4p6"),
E(0,4,37,5,1,"Rb","Rubidium","85.46","312.4","961","alkali metal","[Kr] 5s1"),
E(1,4,38,5,2,"Sr","Strontium","87.62","1050","1650","alkaline earth","[Kr] 5s2"),
E(2,4,39,5,3,"Y","Yttrium","88.90","1799","3203","transition metal","[Kr] 4d1 5s2"),
E(3,4,40,5,4,"Zr","Zirconium","91.22","2128","4650","transition metal","[Kr] 4d2 5s2"),
E(4,4,41,5,5,"Nb","Niobium","92.90","2750","5017","transition metal","[Kr] 4d4 5s1"),
E(5,4,42,5,6,"Mo","Molybdenum","95.95","2896","4912","transition metal","[Kr] 4d5 5s1"),
E(6,4,43,5,7,"Tc","Technetium","98.00","2430","4538","transition metal","[Kr] 4d5 5s2"),
E(7,4,44,5,8,"Ru","Ruthenium","101.07","2607","4423","transition metal","[Kr] 4d7 5s1"),
E(8,4,45,5,9,"Rh","Rhodium","102.90","2237","3968","transition metal","[Kr] 4d8 5s1"),
E(9,4,46,5,10,"Pd","Palladium","106.42","1828","3236","transition metal","[Kr] 4d10"),
E(10,4,47,5,11,"Ag","Silver","107.86","1234","2435","transition metal","[Kr] 4d10 5s1"),
E(11,4,48,5,12,"Cd","Cadmium","112.41","594.2","1040","transition metal","[Kr] 4d10 5s2"),
E(12,4,49,5,13,"In","Indium","114.81","429.7","2345","post-transition","[Kr] 4d10 5s2 5p1"),
E(13,4,50,5,14,"Sn","Tin","118.71","505.0","2875","post-transition","[Kr] 4d10 5s2 5p2"),
E(14,4,51,5,15,"Sb","Antimony","121.76","903.7","1908","metalloid","[Kr] 4d10 5s2 5p3"),
E(15,4,52,5,16,"Te","Tellurium","127.60","722.6","1261","metalloid","[Kr] 4d10 5s2 5p4"),
E(16,4,53,5,17,"I","Iodine","126.90","386.8","457.4","dia nonmetal","[Kr] 4d10 5s2 5p5"),
E(17,4,54,5,18,"Xe","Xenon","131.29","161.4","165.0","noble gas","[Kr] 4d10 5s2 5p6"),
E(0,5,55,6,1,"Cs","Cesium","132.90","301.7","944","alkali metal","[Xe] 6s1"),
E(1,5,56,6,2,"Ba","Barium","137.32","1000","2118","alkaline earth","[Xe] 6s2"),
// Lanthanides & Row 6 
E(2,8,57,6,3,"La","Lanthanum","138.90","1193","3737","lanthanide","[Xe] 5d1 6s2"),
E(3,8,58,6,3,"Ce","Cerium","140.11","1068","3716","lanthanide","[Xe] 4f1 5d1 6s2"),
E(4,8,59,6,3,"Pr","Praseodymium","140.90","1208","3403","lanthanide","[Xe] 4f3 6s2"),
E(5,8,60,6,3,"Nd","Neodymium","144.24","1297","3347","lanthanide","[Xe] 4f4 6s2"),
E(6,8,61,6,3,"Pm","Promethium","145.00","1315","3273","lanthanide","[Xe] 4f5 6s2"),
E(7,8,62,6,3,"Sm","Samarium","150.36","1345","2173","lanthanide","[Xe] 4f6 6s2"),
E(8,8,63,6,3,"Eu","Europium","151.96","1099","1802","lanthanide","[Xe] 4f7 6s2"),
E(9,8,64,6,3,"Gd","Gadolinium","157.25","1585","3273","lanthanide","[Xe] 4f7 5d1 6s2"),
E(10,8,65,6,3,"Tb","Terbium","158.92","1629","3396","lanthanide","[Xe] 4f9 6s2"),
E(11,8,66,6,3,"Dy","Dysprosium","162.50","1680","2840","lanthanide","[Xe] 4f10 6s2"),
E(12,8,67,6,3,"Ho","Holmium","164.93","1734","2873","lanthanide","[Xe] 4f11 6s2"),
E(13,8,68,6,3,"Er","Erbium","167.25","1802","3141","lanthanide","[Xe] 4f12 6s2"),
E(14,8,69,6,3,"Tm","Thulium","168.93","1818","2223","lanthanide","[Xe] 4f13 6s2"),
E(15,8,70,6,3,"Yb","Ytterbium","173.04","1097","1469","lanthanide","[Xe] 4f14 6s2"),
E(16,8,71,6,3,"Lu","Lutetium","174.96","1925","3675","lanthanide","[Xe] 4f14 5d1 6s2"),
E(3,5,72,6,4,"Hf","Hafnium","178.49","2506","4876","transition metal","[Xe] 4f14 5d2 6s2"),
E(4,5,73,6,5,"Ta","Tantalum","180.94","3290","5731","transition metal","[Xe] 4f14 5d3 6s2"),
E(5,5,74,6,6,"W","Tungsten","183.84","3695","6203","transition metal","[Xe] 4f14 5d4 6s2"),
E(6,5,75,6,7,"Re","Rhenium","186.20","3459","5869","transition metal","[Xe] 4f14 5d5 6s2"),
E(7,5,76,6,8,"Os","Osmium","190.23","3306","5285","transition metal","[Xe] 4f14 5d6 6s2"),
E(8,5,77,6,9,"Ir","Iridium","192.21","2719","4403","transition metal","[Xe] 4f14 5d7 6s2"),
E(9,5,78,6,10,"Pt","Platinum","195.08","2041","4098","transition metal","[Xe] 4f14 5d9 6s1"),
E(10,5,79,6,11,"Au","Gold","196.96","1337","3243","transition metal","[Xe] 4f14 5d10 6s1"),
E(11,5,80,6,12,"Hg","Mercury","200.59","234.3","629.8","transition metal","[Xe] 4f14 5d10 6s2"),
E(12,5,81,6,13,"Tl","Thallium","204.38","577","1746","post-transition","[Xe] 4f14 5d10 6s2 6p1"),
E(13,5,82,6,14,"Pb","Lead","207.21","600.6","2022","post-transition","[Xe] 4f14 5d10 6s2 6p2"),
E(14,5,83,6,15,"Bi","Bismuth","208.98","544.7","1837","post-transition","[Xe] 4f14 5d10 6s2 6p3"),
E(15,5,84,6,16,"Po","Polonium","209.00","527","1235","post-transition","[Xe] 4f14 5d10 6s2 6p4"),
E(16,5,85,6,17,"At","Astatine","210.00","575","610","metalloid","[Xe] 4f14 5d10 6s2 6p5"),
E(17,5,86,6,18,"Rn","Radon","222.00","202","211.5","noble gas","[Xe] 4f14 5d10 6s2 6p6"),
E(0,6,87,7,1,"Fr","Francium","223.00","300","950","alkali metal","[Rn] 7s1"),
E(1,6,88,7,2,"Ra","Radium","226.00","1233","2010","alkaline earth","[Rn] 7s2"),
// Actinides & Row 7
E(2,9,89,7,3,"Ac","Actinium","227.00","1500","3500","actinide","[Rn] 6d1 7s2"),
E(3,9,90,7,3,"Th","Thorium","232.03","2023","5061","actinide","[Rn] 6d2 7s2"),
E(4,9,91,7,3,"Pa","Protactinium","231.03","1841","4300","actinide","[Rn] 5f2 6d1 7s2"),
E(5,9,92,7,3,"U","Uranium","238.02","1405","4404","actinide","[Rn] 5f3 6d1 7s2"),
E(6,9,93,7,3,"Np","Neptunium","237.00","912","4447","actinide","[Rn] 5f4 6d1 7s2"),
E(7,9,94,7,3,"Pu","Plutonium","244.00","912.5","3505","actinide","[Rn] 5f6 7s2"),
E(8,9,95,7,3,"Am","Americium","243.00","1449","2880","actinide","[Rn] 5f7 7s2"),
E(9,9,96,7,3,"Cm","Curium","247.00","1613","3383","actinide","[Rn] 5f7 6d1 7s2"),
E(10,9,97,7,3,"Bk","Berkelium","247.00","1259","2900","actinide","[Rn] 5f9 7s2"),
E(11,9,98,7,3,"Cf","Californium","251.00","1173","1743","actinide","[Rn] 5f10 7s2"),
E(12,9,99,7,3,"Es","Einsteinium","252.00","1133","1269","actinide","[Rn] 5f11 7s2"),
E(13,9,100,7,3,"Fm","Fermium","257.00","1800","N/A","actinide","[Rn] 5f12 7s2"),
E(14,9,101,7,3,"Md","Mendelevium","258.00","1100","N/A","actinide","[Rn] 5f13 7s2"),
E(15,9,102,7,3,"No","Nobelium","259.00","1100","N/A","actinide","[Rn] 5f14 7s2"),
E(16,9,103,7,3,"Lr","Lawrencium","266.00","1900","N/A","actinide","[Rn] 5f14 7s2 7p1"),
E(3,6,104,7,4,"Rf","Rutherfordium","267.0","2400","5800","transition metal","[Rn] 5f14 6d2 7s2"),
E(4,6,105,7,5,"Db","Dubnium","268.0","N/A","N/A","transition metal","*[Rn] 5f14 6d3 7s2"),
E(5,6,106,7,6,"Sg","Seaborgium","269.0","N/A","N/A","transition metal","*[Rn] 5f14 6d4 7s2"),
E(6,6,107,7,7,"Bh","Bohrium","270.0","N/A","N/A","transition metal","*[Rn] 5f14 6d5 7s2"),
E(7,6,108,7,8,"Hs","Hassium","269.0","126","N/A","transition metal","*[Rn] 5f14 6d6 7s2"),
E(8,6,109,7,9,"Mt","Meitnerium","278.0","N/A","N/A","transition metal","*[Rn] 5f14 6d7 7s2"),
E(9,6,110,7,10,"Ds","Darmstadtium","281.0","N/A","N/A","transition metal","*[Rn] 5f14 6d9 7s1"),
E(10,6,111,7,11,"Rg","Roentgenium","282.0","N/A","N/A","transition metal","*[Rn] 5f14 6d10 7s1"),
E(11,6,112,7,12,"Cn","Copernicium","285.0","N/A","3570","transition metal","*[Rn] 5f14 6d10 7s2"),
E(12,6,113,7,13,"Nh","Nihonium","286.0","700","1430","transition metal","*[Rn] 5f14 6d10 7s2 7p1"),
E(13,6,114,7,14,"Fl","Flerovium","289.0","340","420","post-transition","*[Rn] 5f14 6d10 7s2 7p2"),
E(14,6,115,7,15,"Mc","Moscovium","289.0","670","1400","post-transition","*[Rn] 5f14 6d10 7s2 7p3"),
E(15,6,116,7,16,"Lv","Livermorium","293.0","709","1085","post-transition","*[Rn] 5f14 6d10 7s2 7p4"),
E(16,6,117,7,17,"Ts","Tennessine","294.0","723","883","metalloid","*[Rn] 5f14 6d10 7s2 7p5"),
E(17,6,118,7,18,"Og","Oganesson","294.0","N/A","350","noble gas","*[Rn] 5f14 6d10 7s2 7p6"),
E(0,7,119,8,1,"Uue","Ununennium","315.0","N/A","630","alkali metal","*[Uuo] 8s1")
};

static const Element* get_elm_xy(uint8_t c, uint8_t r) {
    for(uint16_t i=0; i<119; i++) if(DB[i].col == c && DB[i].row == r) return &DB[i];
    return NULL; 
}

static const Element* get_elm_z(uint8_t z) {
    for(uint16_t i=0; i<119; i++) if(DB[i].z == z) return &DB[i];
    return NULL;
}

typedef struct { uint8_t cur_x; uint8_t cur_y; } ViewModel;

typedef struct {
    Gui* gui; ViewDispatcher* disp; View* v_splash; View* v_grid;         
    View* v_exit_splash; TextBox* v_details; TextBox* v_about;    
    DialogEx* dialog; FuriTimer* t_splash; FuriTimer* t_exit; char buf[1024]; 
} App;

// ========================= splash-screen-entrance ========================
static void start_splash_draw(Canvas* c, void* m) {
    UNUSED(m);
    canvas_set_color(c, ColorBlack); canvas_draw_box(c, 0, 0, 128, 64); 
    canvas_set_color(c, ColorWhite);   
    canvas_set_font(c, FontPrimary); 
    canvas_draw_str_aligned(c, 64, 15, AlignCenter, AlignCenter, "Periodic Table");
    canvas_set_font(c, FontSecondary);
    canvas_draw_str_aligned(c, 64, 30, AlignCenter, AlignCenter, "of Elements");
    canvas_draw_str_aligned(c, 64, 52, AlignCenter, AlignCenter, "LAB-66@Siarhei Besarab");
}
static void s_t_cb(void* ctx) { view_dispatcher_send_custom_event(((App*)ctx)->disp, EVENT_START_APP); }
static void s_en_cb(void* ctx) { furi_timer_start(((App*)ctx)->t_splash, furi_ms_to_ticks(2000)); }


// ========================= splash-screen-exit ========================
static void exit_splash_draw(Canvas* c, void* ctx) {
    UNUSED(ctx);
    canvas_set_color(c, ColorBlack); canvas_draw_box(c, 0, 0, 128, 64);
    canvas_set_color(c, ColorWhite);
    canvas_set_font(c, FontPrimary); 
    canvas_draw_str_aligned(c, 64, 25, AlignCenter, AlignCenter, "See you later!");
    canvas_set_font(c, FontSecondary); 
    canvas_draw_str_aligned(c, 64, 45, AlignCenter, AlignCenter, "LAB-66 (C) 2026"); 
}
static void e_t_cb(void* ctx) { view_dispatcher_send_custom_event(((App*)ctx)->disp, EVENT_FINAL_CLOSE); }
static void e_en_cb(void* ctx) { furi_timer_start(((App*)ctx)->t_exit, furi_ms_to_ticks(1500)); }

static void diag_res_cb(DialogExResult r, void* ctx) {
    App* app = (App*)ctx;
    if (r == DialogExResultLeft) view_dispatcher_switch_to_view(app->disp, VIEW_GRID);
    else if (r == DialogExResultRight) view_dispatcher_switch_to_view(app->disp, VIEW_EXIT_SPLASH);
}

// ========================== main-table ==========================
static void grid_draw(Canvas* canvas, void* _m) {
    ViewModel* model = (ViewModel*)_m;
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    int s_col = (model->cur_x < 9) ? 0 : 9;
    int e_col = s_col + 8; 
    int s_sz = 5, offset = 6; 

    for(int y = 0; y <= 9; y++) {
        for(int x = s_col; x <= e_col; x++) {
            if (!get_elm_xy(x, y)) continue; 
            int px = (x - s_col) * offset; int py = y * offset + 2; 
            if (y == model->cur_y && x == model->cur_x) canvas_draw_box(canvas, px, py, s_sz, s_sz);
            else canvas_draw_frame(canvas, px, py, s_sz, s_sz);
        }
    }
    
    canvas_draw_line(canvas, 62, 0, 62, 63); 
    const Element* elm = get_elm_xy(model->cur_x, model->cur_y);
    if (elm) {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(canvas, 95, 14, AlignCenter, AlignCenter, elm->symbol);
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 95, 27, AlignCenter, AlignCenter, elm->name);
        char d[32]; snprintf(d, sizeof(d), "Z:%d P:%d G:%d", elm->z, elm->period, elm->group);
        canvas_draw_str_aligned(canvas, 95, 42, AlignCenter, AlignCenter, d);
        snprintf(d, sizeof(d), "M: %s", elm->mass);
        canvas_draw_str_aligned(canvas, 95, 54, AlignCenter, AlignCenter, d);
    }
}

static bool grid_input(InputEvent* ev, void* _ctx) {
    App* app = (App*)_ctx; 
    
    if (ev->key == InputKeyOk) {
        if(ev->type == InputTypeShort) {
            uint8_t tx=0, ty=0;
            with_view_model(app->v_grid, ViewModel* m, { tx=m->cur_x; ty=m->cur_y; }, false);
            if (get_elm_xy(tx, ty)) view_dispatcher_send_custom_event(app->disp, EVENT_SHOW_DETAILS);
            return true;
        } else if (ev->type == InputTypeLong) {
            view_dispatcher_send_custom_event(app->disp, EVENT_SHOW_ABOUT);
            return true;
        }
    }

    if(ev->type == InputTypeShort || ev->type == InputTypeRepeat) {
        with_view_model(app->v_grid, ViewModel* m, {
            if (ev->key == InputKeyRight) {
                const Element* cr = get_elm_xy(m->cur_x, m->cur_y);
                if (cr) { const Element* nx = get_elm_z(cr->z + 1); if(nx) { m->cur_x = nx->col; m->cur_y = nx->row; } }
            }
            else if (ev->key == InputKeyLeft) {
                const Element* cr = get_elm_xy(m->cur_x, m->cur_y);
                if (cr) { const Element* pv = get_elm_z(cr->z - 1); if(pv) { m->cur_x = pv->col; m->cur_y = pv->row; } }
            }
            else if (ev->key == InputKeyUp) {
                uint8_t oy = m->cur_y;
                while(m->cur_y > 0) { m->cur_y--; if(get_elm_xy(m->cur_x, m->cur_y)) break; } 
                if(!get_elm_xy(m->cur_x, m->cur_y)) m->cur_y = oy; 
            }
            else if (ev->key == InputKeyDown) {
                uint8_t oy = m->cur_y;
                while(m->cur_y < 9) { m->cur_y++; if(get_elm_xy(m->cur_x, m->cur_y)) break; } 
                if(!get_elm_xy(m->cur_x, m->cur_y)) m->cur_y = oy; 
            }
        }, true); 
        return true; 
    }
    return false;
}

static uint32_t grid_b(void* ctx) { UNUSED(ctx); return VIEW_EXIT_PROMPT; }
static uint32_t to_grid(void* ctx) { UNUSED(ctx); return VIEW_GRID; }

static bool main_ev(void* ctx, uint32_t ev) {
    App* a = (App*)ctx;
    if (ev == EVENT_START_APP) { view_dispatcher_switch_to_view(a->disp, VIEW_GRID); return true; }
    if (ev == EVENT_FINAL_CLOSE) { view_dispatcher_stop(a->disp); return true; }
    
    // element-details
    if (ev == EVENT_SHOW_DETAILS) {
        uint8_t x=0, y=0;
        with_view_model(a->v_grid, ViewModel* m, { x=m->cur_x; y=m->cur_y; }, false);
        const Element* e = get_elm_xy(x, y);
        if (e) { 
            snprintf(a->buf, sizeof(a->buf), 
                 "[ %s (%s) ] Z:%d\n"
                 "--------------------\n"
                 "\xe2\x80\xa2 CATEGORY: %s\n"
                 "\xe2\x80\xa2 GROUP: %d | PERIOD: %d\n"
                 "\n=== Physical ===\n"
                 "\xe2\x80\xa2 MASS: %s u\n"
                 "\xe2\x80\xa2 MELT: %s K\n"
                 "\xe2\x80\xa2 BOIL: %s K\n"
                 "\n=== Quantum ===\n"
                 "\xe2\x80\xa2 ORBIT:  %s", 
                 e->name, e->symbol, e->z, e->cat, e->group, e->period, e->mass, e->melt, e->boil, e->e_conf);
            text_box_set_text(a->v_details, a->buf);
            view_dispatcher_switch_to_view(a->disp, VIEW_DETAILS);
        } return true;
    }
    // about author
    if (ev == EVENT_SHOW_ABOUT) {
        snprintf(a->buf, sizeof(a->buf), 
                 "Flipper Elements v1.0\n"
                 "\n"
                 "Periodic Table of Elements \nfor Flipper Zero.\n"
                 "Author: Siarhei Besarab \naka steanlab.\n"
                 "\nContacts:\n"
                 "LAB-66: t.me/lab66\n"
                 "Linkedin: @steanlab\n"
                 "Mastodon: @lab66\n\n"
                 "--------------------\n"
                 "Support Development:\n"
                 "patreon.com/steanlab\n"
                 "paypal.me/steanlab\n"
                 "revolut.me/steanlab\n"
                 "github.com/sponsors/steanlab\n"
                 "donorbox.org/donations-for-\n"
                 "lab-66\n"
                 "\nThank You!");
        text_box_set_text(a->v_about, a->buf);
        view_dispatcher_switch_to_view(a->disp, VIEW_ABOUT);
        return true;
    } return false;
}

int32_t flipper_elements_app(void* p) {
    UNUSED(p);

    App* a = malloc(sizeof(App));
    memset(a, 0, sizeof(App)); 

    a->gui = furi_record_open(RECORD_GUI);
    a->disp = view_dispatcher_alloc();
    view_dispatcher_attach_to_gui(a->disp, a->gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_set_event_callback_context(a->disp, a);
    view_dispatcher_set_custom_event_callback(a->disp, main_ev); 

    a->t_splash = furi_timer_alloc(s_t_cb, FuriTimerTypeOnce, a);
    a->v_splash = view_alloc();
    view_set_context(a->v_splash, a); 
    view_set_draw_callback(a->v_splash, start_splash_draw);
    view_set_enter_callback(a->v_splash, s_en_cb);
    view_dispatcher_add_view(a->disp, VIEW_SPLASH, a->v_splash);

    a->v_grid = view_alloc();
    view_set_context(a->v_grid, a); 
    view_allocate_model(a->v_grid, ViewModelTypeLocking, sizeof(ViewModel));
    with_view_model(a->v_grid, ViewModel* m, { m->cur_x=0; m->cur_y=0; }, false); 
    view_set_draw_callback(a->v_grid, grid_draw);
    view_set_input_callback(a->v_grid, grid_input);
    view_set_previous_callback(a->v_grid, grid_b); 
    view_dispatcher_add_view(a->disp, VIEW_GRID, a->v_grid);

    a->v_details = text_box_alloc();
    text_box_set_font(a->v_details, TextBoxFontText);
    View* v_d = text_box_get_view(a->v_details);
    view_set_previous_callback(v_d, to_grid);
    view_dispatcher_add_view(a->disp, VIEW_DETAILS, v_d);

    a->v_about = text_box_alloc();
    text_box_set_font(a->v_about, TextBoxFontText);
    View* v_ab = text_box_get_view(a->v_about);
    view_set_previous_callback(v_ab, to_grid);
    view_dispatcher_add_view(a->disp, VIEW_ABOUT, v_ab);

    a->dialog = dialog_ex_alloc();
    dialog_ex_set_context(a->dialog, a);
    dialog_ex_set_header(a->dialog, "Exit Application?", 64, 18, AlignCenter, AlignCenter);
    dialog_ex_set_left_button_text(a->dialog, "No");
    dialog_ex_set_right_button_text(a->dialog, "Yes");
    dialog_ex_set_result_callback(a->dialog, diag_res_cb);
    view_dispatcher_add_view(a->disp, VIEW_EXIT_PROMPT, dialog_ex_get_view(a->dialog));

    a->t_exit = furi_timer_alloc(e_t_cb, FuriTimerTypeOnce, a);
    a->v_exit_splash = view_alloc();
    view_set_context(a->v_exit_splash, a);
    view_set_draw_callback(a->v_exit_splash, exit_splash_draw);
    view_set_enter_callback(a->v_exit_splash, e_en_cb);
    view_dispatcher_add_view(a->disp, VIEW_EXIT_SPLASH, a->v_exit_splash);


    view_dispatcher_switch_to_view(a->disp, VIEW_SPLASH);
    view_dispatcher_run(a->disp);

    view_dispatcher_remove_view(a->disp, VIEW_SPLASH);
    view_dispatcher_remove_view(a->disp, VIEW_GRID);
    view_dispatcher_remove_view(a->disp, VIEW_DETAILS);
    view_dispatcher_remove_view(a->disp, VIEW_ABOUT);
    view_dispatcher_remove_view(a->disp, VIEW_EXIT_PROMPT);
    view_dispatcher_remove_view(a->disp, VIEW_EXIT_SPLASH);

    view_free(a->v_splash);
    view_free(a->v_grid);
    view_free(a->v_exit_splash);
    text_box_free(a->v_details);
    text_box_free(a->v_about);
    dialog_ex_free(a->dialog); 
    
    furi_timer_free(a->t_splash);
    furi_timer_free(a->t_exit);
    view_dispatcher_free(a->disp);
    furi_record_close(RECORD_GUI);
    free(a); 

    return 0;
}
