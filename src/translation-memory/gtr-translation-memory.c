/*
 * Copyright (C) 2008  Ignacio Casal Quinteiro <nacho.resa@gmail.com>
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gtr-translation-memory.h"

/**
 * gtranslator_translation_memory_store:
 * @obj: a #GtrTranslationMemory
 * @original: the untranslated text
 * @translation: the @original text translated
 *
 * Stores the @original and @translation strings in the database.
 */
gboolean
gtranslator_translation_memory_store (GtrTranslationMemory * obj,
				      const gchar * original,
				      const gchar * translation)
{
  g_return_val_if_fail (GTR_IS_TRANSLATION_MEMORY (obj), FALSE);
  return GTR_TRANSLATION_MEMORY_GET_IFACE (obj)->store (obj, original,
							translation);
}

/* Default implementation */
static gboolean
gtranslator_translation_memory_store_default (GtrTranslationMemory *
					      obj, const gchar * original,
					      const gchar * translation)
{
  g_return_val_if_reached (FALSE);
}

/**
 * gtranslator_translation_memory_lookup:
 * @obj: a #GtrTranslationMemory
 * @phrase: the unstranslated text to search for translations.
 *
 * Looks for the @phrase in the database and gets a list of the #GtrTranslationMemoryMatch.
 *
 * Returns: a list of #GtrTranslationMemoryMatch.
 */
GList *
gtranslator_translation_memory_lookup (GtrTranslationMemory * obj,
				       const gchar * phrase)
{
  g_return_val_if_fail (GTR_IS_TRANSLATION_MEMORY (obj), 0);
  return GTR_TRANSLATION_MEMORY_GET_IFACE (obj)->lookup (obj, phrase);
}

/* Default implementation */
static GList *
gtranslator_translation_memory_lookup_default (GtrTranslationMemory *
					       obj, const gchar * phrase)
{
  g_return_val_if_reached (0);
}

/**
 * gtranslator_translation_memory_set_max_omits:
 * @omits: the number of omits
 *
 * Sets the number of omits used in the search.
 */
void
gtranslator_translation_memory_set_max_omits (GtrTranslationMemory *
					      obj, gsize omits)
{
  g_return_if_fail (GTR_IS_TRANSLATION_MEMORY (obj));
  GTR_TRANSLATION_MEMORY_GET_IFACE (obj)->set_max_omits (obj, omits);
}

/* Default implementation */
static void
gtranslator_translation_memory_set_max_omits_default
  (GtrTranslationMemory * obj, gsize omits)
{
  g_return_if_reached ();
}

/**
 * gtranslator_translation_memory_set_max_delta:
 * @delta: the difference in the length of strings
 *
 * Sets the difference in the length of string for searching in the database.
 */
void
gtranslator_translation_memory_set_max_delta (GtrTranslationMemory *
					      obj, gsize delta)
{
  g_return_if_fail (GTR_IS_TRANSLATION_MEMORY (obj));
  GTR_TRANSLATION_MEMORY_GET_IFACE (obj)->set_max_delta (obj, delta);
}

/* Default implementation */
static void
gtranslator_translation_memory_set_max_delta_default
  (GtrTranslationMemory * obj, gsize omits)
{
  g_return_if_reached ();
}

/**
 * gtranslator_translation_memory_set_max_items:
 * @items: the max item to return in lookup
 *
 * Sets the number of item to return in gtranslator_translation_memory_lookup().
 */
void
gtranslator_translation_memory_set_max_items (GtrTranslationMemory *
					      obj, gint items)
{
  g_return_if_fail (GTR_IS_TRANSLATION_MEMORY (obj));
  GTR_TRANSLATION_MEMORY_GET_IFACE (obj)->set_max_items (obj, items);
}

/* Default implementation */
static void
gtranslator_translation_memory_set_max_items_default
  (GtrTranslationMemory * obj, gint items)
{
  g_return_if_reached ();
}

static void
gtranslator_translation_memory_base_init (GtrTranslationMemoryIface *
					  klass)
{
  static gboolean initialized = FALSE;

  klass->store = gtranslator_translation_memory_store_default;
  klass->lookup = gtranslator_translation_memory_lookup_default;
  klass->set_max_omits = gtranslator_translation_memory_set_max_omits_default;
  klass->set_max_delta = gtranslator_translation_memory_set_max_delta_default;
  klass->set_max_items = gtranslator_translation_memory_set_max_items_default;

  if (!initialized)
    {

      initialized = TRUE;
    }
}

GType
gtranslator_translation_memory_get_type (void)
{
  static GType type = 0;
  if (!type)
    {
      static const GTypeInfo info = {
	sizeof (GtrTranslationMemoryIface),
	(GBaseInitFunc) gtranslator_translation_memory_base_init,
	NULL,
	NULL,
	NULL,
	NULL,
	0,
	0,
	NULL
      };
      type =
	g_type_register_static (G_TYPE_INTERFACE,
				"GtrTranslationMemory", &info, 0);
      g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
    }
  return type;
}