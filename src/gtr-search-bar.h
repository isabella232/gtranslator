#pragma once

#include <dazzle.h>
#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define GTR_TYPE_SEARCH_BAR (gtr_search_bar_get_type())

G_DECLARE_FINAL_TYPE (GtrSearchBar, gtr_search_bar, GTR, SEARCH_BAR, DzlBin)

enum
{
  GTR_SEARCH_BAR_FIND_RESPONSE = 100,
  GTR_SEARCH_BAR_REPLACE_RESPONSE,
  GTR_SEARCH_BAR_REPLACE_ALL_RESPONSE
};

typedef enum
{
  GTR_SEARCH_BAR_MSG_INFO,
  GTR_SEARCH_BAR_MSG_ERROR
} GtrSearchBarMsg;


GtkSearchEntry  *gtr_search_bar_get_search       (GtrSearchBar *self);
void             gtr_search_bar_set_search       (GtrSearchBar *self,
                                                         GtkSearchEntry    *search);
gboolean         gtr_search_bar_get_show_options (GtrSearchBar *self);
void             gtr_search_bar_set_show_options (GtrSearchBar *self,
                                                         gboolean            show_options);
gboolean         gtr_search_bar_get_replace_mode (GtrSearchBar *self);
void             gtr_search_bar_set_replace_mode (GtrSearchBar *self,
                                                  gboolean            replace_mode);
const gchar      *gtr_search_bar_get_search_text (GtrSearchBar *dialog);
void             gtr_search_bar_set_search_text (GtrSearchBar *dialog,
                                                 const gchar *text);


const gchar      *gtr_search_bar_get_replace_text (GtrSearchBar *dialog);
void             gtr_search_bar_set_replace_text (GtrSearchBar *dialog,
                                                  const gchar *text);

gboolean         gtr_search_bar_get_original_text (GtrSearchBar *dialog);
void             gtr_search_bar_set_original_text (GtrSearchBar *dialog,
                                                   gboolean match_case);

gboolean         gtr_search_bar_get_translated_text (GtrSearchBar *dialog);
void             gtr_search_bar_set_translated_text (GtrSearchBar *dialog,
                                                     gboolean match_case);

gboolean         gtr_search_bar_get_match_case (GtrSearchBar *dialog);
void             gtr_search_bar_set_match_case (GtrSearchBar *dialog,
                                                gboolean match_case);

gboolean         gtr_search_bar_get_entire_word (GtrSearchBar * dialog);
void             gtr_search_bar_set_entire_word (GtrSearchBar *dialog,
                                                 gboolean entire_word);

gboolean         gtr_search_bar_get_backwards (GtrSearchBar *dialog);
void             gtr_search_bar_set_backwards (GtrSearchBar *dialog,
                                               gboolean backwards);

gboolean         gtr_search_bar_get_wrap_around (GtrSearchBar *dialog);
void             gtr_search_bar_set_wrap_around (GtrSearchBar *dialog,
                                                 gboolean wrap_around);

void             gtr_search_bar_show_message (GtrSearchBar *dialog,
                                              char *message,
                                              GtrSearchBarMsg info_type);

void             gtr_search_bar_present_with_time (GtrSearchBar * dialog,
                                                   guint32 timestamp);

G_END_DECLS

