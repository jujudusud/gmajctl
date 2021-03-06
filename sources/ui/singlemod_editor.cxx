// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "app_i18n.h"
#include "singlemod_editor.h"
#include "widget_ex.h"
Single_Mod_Editor::Single_Mod_Editor(int X, int Y, int W, int H, const char *L)
  : Fl_Group(X, Y, W, H, L) {
{ sl_assignment = new Fl_Slider_Ex(2, 15, 15, 60);
  sl_assignment->type(2);
  sl_assignment->box(FL_THIN_DOWN_BOX);
  sl_assignment->color(FL_BACKGROUND_COLOR);
  sl_assignment->selection_color(FL_BACKGROUND_COLOR);
  sl_assignment->labeltype(FL_NORMAL_LABEL);
  sl_assignment->labelfont(0);
  sl_assignment->labelsize(10);
  sl_assignment->labelcolor(FL_FOREGROUND_COLOR);
  sl_assignment->align(Fl_Align(FL_ALIGN_TOP));
  sl_assignment->when(FL_WHEN_CHANGED);
} // Fl_Slider_Ex* sl_assignment
{ sl_min = new Fl_Slider_Ex(22, 15, 15, 60);
  sl_min->type(2);
  sl_min->box(FL_THIN_DOWN_BOX);
  sl_min->color(FL_BACKGROUND_COLOR);
  sl_min->selection_color(FL_BACKGROUND_COLOR);
  sl_min->labeltype(FL_NORMAL_LABEL);
  sl_min->labelfont(0);
  sl_min->labelsize(10);
  sl_min->labelcolor(FL_FOREGROUND_COLOR);
  sl_min->align(Fl_Align(FL_ALIGN_TOP));
  sl_min->when(FL_WHEN_CHANGED);
} // Fl_Slider_Ex* sl_min
{ sl_mid = new Fl_Slider_Ex(42, 15, 15, 60);
  sl_mid->type(2);
  sl_mid->box(FL_THIN_DOWN_BOX);
  sl_mid->color(FL_BACKGROUND_COLOR);
  sl_mid->selection_color(FL_BACKGROUND_COLOR);
  sl_mid->labeltype(FL_NORMAL_LABEL);
  sl_mid->labelfont(0);
  sl_mid->labelsize(10);
  sl_mid->labelcolor(FL_FOREGROUND_COLOR);
  sl_mid->align(Fl_Align(FL_ALIGN_TOP));
  sl_mid->when(FL_WHEN_CHANGED);
} // Fl_Slider_Ex* sl_mid
{ sl_max = new Fl_Slider_Ex(62, 15, 15, 60);
  sl_max->type(2);
  sl_max->box(FL_THIN_DOWN_BOX);
  sl_max->color(FL_BACKGROUND_COLOR);
  sl_max->selection_color(FL_BACKGROUND_COLOR);
  sl_max->labeltype(FL_NORMAL_LABEL);
  sl_max->labelfont(0);
  sl_max->labelsize(10);
  sl_max->labelcolor(FL_FOREGROUND_COLOR);
  sl_max->align(Fl_Align(FL_ALIGN_TOP));
  sl_max->when(FL_WHEN_CHANGED);
} // Fl_Slider_Ex* sl_max
{ lbl_assignment = new Fl_Box(0, 0, 20, 15, _("Off"));
  lbl_assignment->labelsize(10);
} // Fl_Box* lbl_assignment
{ lbl_min = new Fl_Box(20, 0, 20, 15, _("100"));
  lbl_min->labelsize(10);
} // Fl_Box* lbl_min
{ lbl_mid = new Fl_Box(40, 0, 20, 15, _("100"));
  lbl_mid->labelsize(10);
} // Fl_Box* lbl_mid
{ lbl_max = new Fl_Box(60, 0, 20, 15, _("100"));
  lbl_max->labelsize(10);
} // Fl_Box* lbl_max
end();
}
