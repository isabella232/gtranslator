#define G_LOG_DOMAIN "gtr-search-bar"

#include "config.h"

#include <dazzle.h>
#include <glib/gi18n.h>

#include "gtr-search-bar.h"
#include "gtr-actions.h"
#include "gtr-tab.h"

struct _GtrSearchBar
{
  DzlBin                  parent_instance;

  DzlSignalGroup          *search_signals;
  DzlBindingGroup         *search_bindings;
  GtkSearchEntry          *search;

  GObject                 *search_entry_tag;

  GtkCheckButton          *case_sensitive;
  GtkButton               *replace_all_button;
  GtkButton               *replace_button;
  GtkButton               *close_button;
  GtkButton               *previous_button;
  GtkButton               *next_button;
  GtkEntry                *replace_entry;
  GtkEntry                *search_entry;
  GtkGrid                 *search_options;
  GtkCheckButton          *whole_word;
  GtkCheckButton          *wrap_around_button;
  GtkCheckButton          *original_text;
  GtkCheckButton          *translated_text;
  GtkLabel                *search_text_error;
  GtkWidget               *info_revealer;
  //GtkWidget               *info_label;

  GtrWindow               *active_window;

  guint                    match_source;

  guint                    show_options : 1;
  guint                    replace_mode : 1;
};

enum {
  PROP_0,
  PROP_REPLACE_MODE,
  PROP_SHOW_OPTIONS,
  PROP_SHOW_CLOSE_BUTTON,
  N_PROPS
};

enum {
  STOP_SEARCH,
  N_SIGNALS
};

G_DEFINE_TYPE (GtrSearchBar, gtr_search_bar, DZL_TYPE_BIN)

static GParamSpec *properties [N_PROPS];
static guint signals [N_SIGNALS];

/*----------------------------------------NEW RE-WRITTEN METHODS--------------------------------------*/

void
gtr_search_bar_set_search_text (GtrSearchBar *dialog,
                                const gchar  *text)
{
  g_return_if_fail (GTR_IS_SEARCH_BAR (dialog));
  g_return_if_fail (text != NULL);

  gtk_entry_set_text (GTK_ENTRY (dialog->search_entry), text);

  gtk_dialog_set_response_sensitive (GTK_DIALOG (dialog),
                                     GTR_SEARCH_BAR_FIND_RESPONSE,
                                     (*text != '\0'));

  gtk_dialog_set_response_sensitive (GTK_DIALOG (dialog),
                                     GTR_SEARCH_BAR_REPLACE_ALL_RESPONSE,
                                     (*text != '\0'));
}

/*
 * The text must be unescaped before searching.
 */
const gchar*
gtr_search_bar_get_search_text (GtrSearchBar *dialog)
{
  //g_assert (GTR_IS_SEARCH_BAR (dialog));
  g_return_val_if_fail (GTR_IS_SEARCH_BAR (dialog), NULL);

  return gtk_entry_get_text (GTK_ENTRY (dialog->search_entry));
}

void
gtr_search_bar_set_replace_text (GtrSearchBar *dialog,
                                 const gchar  *text)
{
  //g_assert (GTR_IS_SEARCH_BAR (dialog));
  g_return_if_fail (GTR_IS_SEARCH_BAR (dialog));
  g_return_if_fail (text != NULL);

  gtk_entry_set_text (GTK_ENTRY (dialog->replace_entry), text);
}

const gchar *
gtr_search_bar_get_replace_text (GtrSearchBar *dialog)
{
  g_return_val_if_fail (GTR_IS_SEARCH_BAR (dialog), NULL);

  return gtk_entry_get_text (GTK_ENTRY (dialog->replace_entry));
}

void
gtr_search_bar_set_original_text (GtrSearchBar *dialog,
                                  gboolean match_case)
{
  g_return_if_fail (GTR_IS_SEARCH_BAR (dialog));

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON
                                (dialog->original_text),
                                match_case);
}

gboolean
gtr_search_bar_get_original_text (GtrSearchBar *dialog)
{
  g_return_val_if_fail (GTR_IS_SEARCH_BAR (dialog), FALSE);

  return
    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
                                  (dialog->original_text));
}

void
gtr_search_bar_set_translated_text (GtrSearchBar *dialog,
                                    gboolean match_case)
{

  g_return_if_fail (GTR_IS_SEARCH_BAR (dialog));

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON
                                (dialog->translated_text),
                                match_case);
}

gboolean
gtr_search_bar_get_translated_text (GtrSearchBar *dialog)
{

  g_return_val_if_fail (GTR_IS_SEARCH_BAR (dialog), FALSE);

  return gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (dialog->translated_text));
}

void
gtr_search_bar_set_match_case (GtrSearchBar *dialog,
                               gboolean match_case)
{

  g_return_if_fail (GTR_IS_SEARCH_BAR (dialog));

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog->case_sensitive),
                                match_case);
}

gboolean
gtr_search_bar_get_match_case (GtrSearchBar *dialog)
{
  g_return_val_if_fail (GTR_IS_SEARCH_BAR (dialog), FALSE);

  return
    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
                                  (dialog->case_sensitive));
}

void
gtr_search_bar_set_entire_word (GtrSearchBar *dialog,
                                gboolean entire_word)
{
  g_return_if_fail (GTR_IS_SEARCH_BAR (dialog));

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON
                                (dialog->whole_word),
                                entire_word);
}

gboolean
gtr_search_bar_get_entire_word (GtrSearchBar *dialog)
{

  g_return_val_if_fail (GTR_IS_SEARCH_BAR (dialog), FALSE);

  return
    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
                                  (dialog->whole_word));
}

void
gtr_search_bar_set_backwards (GtrSearchBar *dialog, gboolean backwards)
{

  g_return_if_fail (GTR_IS_SEARCH_BAR (dialog));

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON
                                (dialog->previous_button),
                                backwards);
}

gboolean
gtr_search_bar_get_backwards (GtrSearchBar *dialog)
{

  g_return_val_if_fail (GTR_IS_SEARCH_BAR (dialog), FALSE);

  return
    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
                                  (dialog->previous_button));
}

void
gtr_search_bar_set_wrap_around (GtrSearchBar *dialog,
                                   gboolean wrap_around)
{

  g_return_if_fail (GTR_IS_SEARCH_BAR (dialog));

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON
                                (dialog->wrap_around_button),
                                wrap_around);
}

gboolean
gtr_search_bar_get_wrap_around (GtrSearchBar *dialog)
{
  g_return_val_if_fail (GTR_IS_SEARCH_BAR (dialog), FALSE);

  return
    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON
                                  (dialog->wrap_around_button));
}

/*
void
gtr_search_bar_show_message (GtrSearchBar *dialog,
                             char *message,
                             GtrSearchBarMsg info_type)
{

  GtkStyleContext *ctx = gtk_widget_get_style_context (dialog->info_label);

  if (!message)
    {
      gtk_revealer_set_reveal_child (GTK_REVEALER (dialog->info_revealer), FALSE);
      return;
    }

  gtk_label_set_text (GTK_LABEL (dialog->info_label), message);
  gtk_revealer_set_reveal_child (GTK_REVEALER (dialog->info_revealer), TRUE);

  switch (info_type)
    {
    case GTR_SEARCH_BAR_MSG_ERROR:
      gtk_style_context_remove_class (ctx, "info");
      gtk_style_context_add_class (ctx, "error");
      break;
    case GTR_SEARCH_BAR_MSG_INFO:
    default:
      gtk_style_context_remove_class (ctx, "error");
      gtk_style_context_add_class (ctx, "info");
      break;
    }
}
*/

/*-----------------------------END OF NEW RE_WRITTEN METHODS-----------------*/

gboolean
gtr_search_bar_get_replace_mode (GtrSearchBar *self)
{
  g_return_val_if_fail (GTK_IS_SEARCH_BAR (self), FALSE);

  return self->replace_mode;
}

void
gtr_search_bar_set_replace_mode (GtrSearchBar *self,
                                 gboolean      replace_mode)
{
    g_return_if_fail (GTR_IS_SEARCH_BAR (self));

  replace_mode = !!replace_mode;

  if (replace_mode != self->replace_mode)
    {
      self->replace_mode = replace_mode;
      gtk_widget_set_visible (GTK_WIDGET (self->replace_entry), replace_mode);
      gtk_widget_set_visible (GTK_WIDGET (self->replace_button), replace_mode);
      gtk_widget_set_visible (GTK_WIDGET (self->replace_all_button), replace_mode);
      g_object_notify_by_pspec (G_OBJECT (self), properties [PROP_REPLACE_MODE]);
    }
}

static gboolean
maybe_escape_regex (GBinding     *binding,
                    const GValue *from_value,
                    GValue       *to_value,
                    gpointer      user_data)
{
  GtrSearchBar *self = user_data;
  const gchar *entry_text;

  g_assert (GTR_IS_SEARCH_BAR (self));
  g_assert (from_value != NULL);
  g_assert (to_value != NULL);

  entry_text = g_value_get_string (from_value);

  if (entry_text == NULL)
    {
      g_value_set_static_string (to_value, "");
    }
  else
    {
      g_autofree gchar *unescaped = NULL;

      if (self->search != NULL //&& !ide_editor_search_get_regex_enabled (self->search)
          )
        entry_text = unescaped = gtk_source_utils_unescape_search_text (entry_text);

      g_value_set_string (to_value, entry_text);
    }

  return TRUE;
}

static gboolean
pacify_null_text (GBinding     *binding,
                  const GValue *from_value,
                  GValue       *to_value,
                  gpointer      user_data)
{
  g_assert (from_value != NULL);
  g_assert (to_value != NULL);
  g_assert (G_VALUE_HOLDS_STRING (from_value));
  g_assert (G_VALUE_HOLDS_STRING (to_value));

  if (g_value_get_string (from_value) == NULL)
    g_value_set_static_string (to_value, "");
  else
    g_value_copy (from_value, to_value);

  return TRUE;
}

static void
gtr_search_bar_grab_focus (GtkWidget *widget)
{
  GtrSearchBar *self = (GtrSearchBar *)widget;

  g_assert (GTR_IS_SEARCH_BAR (self));

  gtk_widget_grab_focus (GTK_WIDGET (self->search_entry));
}

static void
search_entry_populate_popup (GtrSearchBar *self,
                             GtkWidget    *widget,
                             GtkEntry     *entry)
{
  g_assert (GTR_IS_SEARCH_BAR (self));
  g_assert (GTK_IS_MENU (widget));
  g_assert (GTK_IS_ENTRY (entry));

  if (GTK_IS_MENU (widget))
    {
      g_autoptr(DzlPropertiesGroup) group = NULL;

      GtkWidget *item;
      GtkWidget *sep;
      guint pos = 0;

      item = gtk_check_menu_item_new_with_label (_("Case sensitive"));
      gtk_actionable_set_action_name (GTK_ACTIONABLE (item), "search-settings.case-sensitive");
      gtk_menu_shell_insert (GTK_MENU_SHELL (widget), item, pos++);
      gtk_widget_show (item);

      item = gtk_check_menu_item_new_with_label (_("Match whole word only"));
      gtk_actionable_set_action_name (GTK_ACTIONABLE (item), "search-settings.at-word-boundaries");
      gtk_menu_shell_insert (GTK_MENU_SHELL (widget), item, pos++);
      gtk_widget_show (item);

      item = gtk_check_menu_item_new_with_label (_("Wrap around"));
      gtk_actionable_set_action_name (GTK_ACTIONABLE (item), "search-settings.wrap-around");
      gtk_menu_shell_insert (GTK_MENU_SHELL (widget), item, pos++);
      gtk_widget_show (item);

      item = gtk_check_menu_item_new_with_label (_("Original text"));
      gtk_actionable_set_action_name (GTK_ACTIONABLE (item), "search-settings.at-original-text");
      gtk_menu_shell_insert (GTK_MENU_SHELL (widget), item, pos++);
      gtk_widget_show (item);

      item = gtk_check_menu_item_new_with_label (_("Translated text"));
      gtk_actionable_set_action_name (GTK_ACTIONABLE (item), "search-settings.at-translated-text");
      gtk_menu_shell_insert (GTK_MENU_SHELL (widget), item, pos++);
      gtk_widget_show (item);

      sep = gtk_separator_menu_item_new ();
      gtk_menu_shell_insert (GTK_MENU_SHELL (widget), sep, pos++);
      gtk_widget_show (sep);

      if (self->search != NULL)
        {
          group = dzl_properties_group_new (G_OBJECT (self->search));
          dzl_properties_group_add_all_properties (group);
        }

      gtk_widget_insert_action_group (widget, "search-settings", G_ACTION_GROUP (group));
    }
}

static void
gtr_search_bar_real_stop_search (GtrSearchBar *self)
{
  g_assert (GTR_IS_SEARCH_BAR (self));
}

static void
search_entry_stop_search (GtrSearchBar *self,
                          GtkSearchEntry     *entry)
{
  g_assert (GTR_IS_SEARCH_BAR (self));
  g_assert (GTK_IS_SEARCH_ENTRY (entry));

  g_signal_emit (self, signals [STOP_SEARCH], 0);
}

//static void
//search_entry_previous_match (GtrSearchBar *self,
//                             GtkSearchEntry     *entry)

//static void
//search_entry_next_match (GtrSearchBar *self,
//                           GtkSearchEntry     *entry)

//static void
//search_entry_activate (GtrSearchBar *self,
//                       GtkEntry     *entry)

//static void
//search_entry_changed (GtrSearchBar *self,
//                      IdeTaggedEntry     *entry)

//static gboolean
//update_match_positions (gpointer user_data)

//static void
//gtr_search_bar_notify_match (GtrSearchBar *self,
//                             GParamSpec         *pspec,
//                               GtkEntry    *search)

static void
gtr_search_bar_destroy (GtkWidget *widget)
{
  GtrSearchBar *self = (GtrSearchBar *)widget;

  dzl_clear_source (&self->match_source);

  g_clear_object (&self->search_signals);
  g_clear_object (&self->search_bindings);
  g_clear_object (&self->search);
  g_clear_object (&self->search_entry_tag);

  GTK_WIDGET_CLASS (gtr_search_bar_parent_class)->destroy (widget);
}

gboolean
gtr_search_bar_get_show_options (GtrSearchBar *self)
{
  g_return_val_if_fail (GTR_IS_SEARCH_BAR (self), FALSE);

  return self->show_options;
}

void
gtr_search_bar_set_show_options (GtrSearchBar *self,
                                 gboolean      show_options)
{
  g_return_if_fail (GTR_IS_SEARCH_BAR (self));

  show_options = !!show_options;

  if (self->show_options != show_options)
    {
      self->show_options = show_options;
      gtk_widget_set_visible (GTK_WIDGET (self->search_options), show_options);
      g_object_notify_by_pspec (G_OBJECT (self), properties [PROP_SHOW_OPTIONS]);
    }
}

static void
gtr_search_bar_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  GtrSearchBar *self = GTR_SEARCH_BAR (object);

  switch (prop_id)
    {
    case PROP_REPLACE_MODE:
      g_value_set_boolean (value, gtr_search_bar_get_replace_mode (self));
      break;

    case PROP_SHOW_OPTIONS:
      g_value_set_boolean (value, gtr_search_bar_get_show_options (self));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gtr_search_bar_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  GtrSearchBar *self = GTR_SEARCH_BAR (object);

  switch (prop_id)
    {
    case PROP_REPLACE_MODE:
      gtr_search_bar_set_replace_mode (self, g_value_get_boolean (value));
      break;

    case PROP_SHOW_OPTIONS:
      gtr_search_bar_set_show_options (self, g_value_get_boolean (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gtr_search_bar_class_init (GtrSearchBarClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->get_property = gtr_search_bar_get_property;
  object_class->set_property = gtr_search_bar_set_property;

 // widget_class->destroy = gtr_search_bar_destroy;
 // widget_class->grab_focus = gtr_search_bar_grab_focus;

   properties [PROP_REPLACE_MODE] =
    g_param_spec_boolean ("replace-mode", NULL, NULL, FALSE,
                          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  properties [PROP_SHOW_OPTIONS] =
    g_param_spec_boolean ("show-options", NULL, NULL, FALSE,
                          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, N_PROPS, properties);

  signals [STOP_SEARCH] =
    g_signal_new_class_handler ("stop-search",
                                G_TYPE_FROM_CLASS (klass),
                                G_SIGNAL_RUN_LAST,
                                G_CALLBACK (gtr_search_bar_real_stop_search),
                                NULL, NULL,
                                g_cclosure_marshal_VOID__VOID,
                                G_TYPE_NONE, 0);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/translator/gtr-search-bar.ui");
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, case_sensitive);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, replace_all_button);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, replace_button);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, replace_entry);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, search_entry);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, previous_button);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, next_button);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, search_options);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, search_text_error);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, whole_word);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, wrap_around_button );
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, close_button);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, original_text);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, translated_text);
  gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, info_revealer);
  //gtk_widget_class_bind_template_child (widget_class, GtrSearchBar, info_label);

  gtk_widget_class_set_css_name (widget_class, "gtrsearchbar");
}

static void
gtr_search_bar_init (GtrSearchBar *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->search_signals = dzl_signal_group_new (GTK_TYPE_SEARCH_ENTRY);

/*  dzl_signal_group_connect_swapped (self->search_signals,
                                    "notify::match-count",
                                    G_CALLBACK (ide_editor_search_bar_notify_match),
                                    self);

  dzl_signal_group_connect_swapped (self->search_signals,
                                    "notify::match-position",
                                    G_CALLBACK (ide_editor_search_bar_notify_match),
                                    self);
*/

  self->search_bindings = dzl_binding_group_new ();

  dzl_binding_group_bind_full (self->search_bindings, "search-text",
                               self->search_entry, "text",
                               G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL,
                               maybe_escape_regex, pacify_null_text, self, NULL);

  dzl_binding_group_bind_full (self->search_bindings, "replacement-text",
                               self->replace_entry, "text",
                               G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL,
                               pacify_null_text, pacify_null_text, NULL, NULL);

  dzl_binding_group_bind (self->search_bindings, "case-sensitive",
                          self->case_sensitive, "active",
                          G_BINDING_SYNC_CREATE |
 G_BINDING_BIDIRECTIONAL);

 dzl_binding_group_bind (self->search_bindings, "at-word-boundaries",
                          self->whole_word, "active",
                          G_BINDING_SYNC_CREATE | G_BINDING_BIDIRECTIONAL);

// TODO:
//  g_signal_connect_swapped (self->search_entry,
//                            "activate",
//                            G_CALLBACK (search_entry_activate),
//                            self);

// TODO:
// struct data *cb_data = g_new0(struct data, 1);
//cb_data->tab = gtk_entry_new();
//cb_data->foo = "1;
// struct data *cb_data = g_new0(struct data, 1);
// cb_data->entry = gtk_entry_new();
// cb_data->count = 1;
// g_signal_connect_data (self->search_entry,
//                         "changed",
//                         G_CALLBACK (search_entry_changed),
//                         self, NULL,
//                         G_CONNECT_SWAPPED | G_CONNECT_AFTER);

  g_signal_connect_swapped (self->search_entry,
                            "populate-popup",
                            G_CALLBACK (search_entry_populate_popup),
                            self);

// TODO:
//
//
//  g_signal_connect_swapped (self->close_button,
//                            "clicked",
//                            G_CALLBACK (gtr_tab_show_hide_search_bar),
//                            0);

  g_signal_connect_swapped (self->search_entry,
                            "stop-search",
                            G_CALLBACK (search_entry_stop_search),
                            self);

// TODO:
// g_signal_connect_swapped (self->search_entry,
//                            "previous-match",
//                            G_CALLBACK  (search_entry_previous_match),
//                            self);

// TODO:
// g_signal_connect_swapped (self->search_entry,
//                            "next-match",
//                            G_CALLBACK (search_entry_next_match),
//                            self);

// _ide_editor_search_bar_init_shortcuts (self);
}

GtkSearchEntry *
gtr_search_bar_get_search (GtrSearchBar *self)
{
  g_return_val_if_fail (GTR_IS_SEARCH_BAR (self), NULL);

  return self->search;
}

void
gtr_search_bar_set_search (GtrSearchBar *self,
                           GtkSearchEntry    *search)
{
  g_return_if_fail (GTR_IS_SEARCH_BAR (self));

  if (g_set_object (&self->search, search))
    {
      dzl_signal_group_set_target (self->search_signals, search);
      dzl_binding_group_set_source (self->search_bindings, search);
    }
}


