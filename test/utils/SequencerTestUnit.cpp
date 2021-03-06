//
// Created by sascha on 12/23/16.
//

#include <matador/utils/sequencer.hpp>
#include "SequencerTestUnit.hpp"

SequencerTestUnit::SequencerTestUnit() : unit_test("sequencer", "sequencer test unit")
{
  add_test("init", std::bind(&SequencerTestUnit::test_init, this), "init sequencer");
  add_test("inc", std::bind(&SequencerTestUnit::test_inc, this), "increment sequencer");
  add_test("reset", std::bind(&SequencerTestUnit::test_reset, this), "reset sequencer");
  add_test("exchange", std::bind(&SequencerTestUnit::test_reset, this), "exchange sequencer");
}

void SequencerTestUnit::test_init()
{
  matador::sequencer seq;

  UNIT_ASSERT_EQUAL(seq.init(), 0UL);
}

void SequencerTestUnit::test_inc()
{
  matador::sequencer seq;

  seq.init();

  UNIT_ASSERT_EQUAL(seq.current(), 0UL);
  UNIT_ASSERT_EQUAL(seq.next(), 1UL);
  UNIT_ASSERT_EQUAL(seq.update(5), 5UL);
}

void SequencerTestUnit::test_reset()
{
  matador::sequencer seq;

  seq.init();

  UNIT_ASSERT_EQUAL(seq.current(), 0UL);
  UNIT_ASSERT_EQUAL(seq.next(), 1UL);
  UNIT_ASSERT_EQUAL(seq.reset(3), 3UL);
}

void SequencerTestUnit::test_exchange()
{
  matador::sequencer seq;

  seq.init();

  UNIT_ASSERT_EQUAL(seq.current(), 0UL);
  UNIT_ASSERT_EQUAL(seq.next(), 1UL);

  matador::sequencer_impl_ptr another_seq_ptr(new matador::default_sequencer);

  auto old_seq_ptr = seq.exchange_sequencer(another_seq_ptr);

  UNIT_ASSERT_EQUAL(seq.current(), 0UL);
  UNIT_ASSERT_EQUAL(seq.next(), 1UL);
}
