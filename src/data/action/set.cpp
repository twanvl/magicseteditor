//+----------------------------------------------------------------------------+
//| Description:  Magic Set Editor - Program to make Magic (tm) cards          |
//| Copyright:    (C) 2001 - 2006 Twan van Laarhoven                           |
//| License:      GNU General Public License 2 or later (see file COPYING)     |
//+----------------------------------------------------------------------------+

// ----------------------------------------------------------------------------- : Includes

#include <data/action/set.hpp>
#include <data/set.hpp>
#include <data/card.hpp>
#include <util/error.hpp>

// ----------------------------------------------------------------------------- : Add card

AddCardAction::AddCardAction(Set& set)
	: CardListAction(set), card(new Card(*set.game))
{}

AddCardAction::AddCardAction(Set& set, const CardP& card)
	: CardListAction(set), card(card)
{}

String AddCardAction::getName(bool to_undo) const {
	return _("Add card");
}

void AddCardAction::perform(bool to_undo) {
	if (!to_undo) {
		set.cards.push_back(card);
	} else {
		assert(!set.cards.empty());
		set.cards.pop_back();
	}
}


// ----------------------------------------------------------------------------- : Remove card

RemoveCardAction::RemoveCardAction(Set& set, const CardP& card)
	: CardListAction(set), card(card)
{
	// find the card_id of the card we want to remove
	vector<CardP>::iterator it = find(set.cards.begin(), set.cards.end(), card);
	if (it != set.cards.end()) {
		card_id = it - set.cards.begin();
	} else {
		throw InternalError(_("Card to remove not found in set"));
	}
}

String RemoveCardAction::getName(bool to_undo) const {
	return _("Remove card");
}

void RemoveCardAction::perform(bool to_undo) {
	if (!to_undo) {
		assert(card_id < set.cards.size());
		set.cards.erase(set.cards.begin() + card_id);
	} else {
		assert(card_id <= set.cards.size());
		set.cards.insert(set.cards.begin() + card_id, card);
	}
}


// ----------------------------------------------------------------------------- : Reorder cards

ReorderCardsAction::ReorderCardsAction(Set& set, size_t card_id1, size_t card_id2)
	: CardListAction(set), card_id1(card_id1), card_id2(card_id2)
{}

String ReorderCardsAction::getName(bool to_undo) const {
	return _("Reorder cards");
}

void ReorderCardsAction::perform(bool to_undo) {
	assert(card_id1 < set.cards.size());
	assert(card_id2 < set.cards.size());
	swap(set.cards[card_id1], set.cards[card_id2]);
}