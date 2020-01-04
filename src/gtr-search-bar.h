#pragma once

#include <dazzle.h>
#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define GTR_TYPE_SEARCH_BAR (gtr_search_bar_get_type())


G_DECLARE_FINAL_TYPE (GtrSearchBar, gtr_search_bar, GTR, SEARCH_BAR, DzlBin)


GtkSearchEntry  *gtr_search_bar_get_search       (GtrSearchBar *self);
void             gtr_search_bar_set_search       (GtrSearchBar *self,
                                                         GtkSearchEntry    *search);
gboolean         gtr_search_bar_get_show_options (GtrSearchBar *self);
void             gtr_search_bar_set_show_options (GtrSearchBar *self,
                                                         gboolean            show_options);
gboolean         gtr_search_bar_get_replace_mode (GtrSearchBar *self);
void             gtr_search_bar_set_replace_mode (GtrSearchBar *self,
                                                         gboolean            replace_mode);

G_END_DECLS

