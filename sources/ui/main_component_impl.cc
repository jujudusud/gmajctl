//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "main_component.h"
#include "patch_chooser.h"
#include "association.h"
#include "app_i18n.h"
#include "device/midi.h"
#include "model/patch.h"
#include "model/patch_loader.h"
#include "model/patch_writer.h"
#include "model/parameter.h"
#include "utility/misc.h"
#include <FL/Fl_Dial.H>
#include <FL/Fl_Native_File_Chooser.H>

void Main_Component::init()
{
    Patch_Bank *pbank = new Patch_Bank;
    pbank_.reset(pbank);

    P_General *pgen = new P_General;
    pgen_.reset(pgen);

    set_nth_patch(0, Patch::create_empty());

    update_midi_outs();
}

Main_Component::~Main_Component()
{
}

unsigned Main_Component::get_patch_number() const
{
    int value = br_bank->value();
    if (value == 0)
        return ~0u;
    return (unsigned)(uintptr_t)br_bank->data(value);
}

void Main_Component::set_patch_number(unsigned no)
{
    br_bank->select_only((void *)(uintptr_t)no);
}

void Main_Component::set_nth_patch(unsigned nth, const Patch &pat)
{
    if (nth >= Patch_Bank::max_count)
        return;

    Patch_Bank &pbank = *pbank_;
    pbank.slot[nth] = pat;
    pbank.slot[nth].patch_number(nth);
    pbank.used.set(nth);

    refresh_bank_browser();
}

void Main_Component::refresh_bank_browser()
{
    Fl_Browser &br = *br_bank;
    Patch_Bank &pbank = *pbank_;

    br.clear();
    for (unsigned i = 0; i < Patch_Bank::max_count; ++i) {
        if (pbank.used[i]) {
            std::string text = std::to_string(i + 1) + " - " + pbank.slot[i].name();
            br.add(text.c_str(), (void *)(uintptr_t)i);
        }
    }
}

void Main_Component::refresh_patch_display()
{
    unsigned patchno = get_patch_number();
    if (patchno == ~0u)
        return;
    const Patch &pat = pbank_->slot[patchno];

    P_General &pgen = *pgen_;
    chk_compressor->value(pgen.enable_compressor().get(pat));
    chk_filter->value(pgen.enable_filter().get(pat));
    chk_pitch->value(pgen.enable_pitch().get(pat));
    chk_chorus->value(pgen.enable_modulator().get(pat));
    chk_delay->value(pgen.enable_delay().get(pat));
    chk_reverb->value(pgen.enable_reverb().get(pat));
    chk_equalizer->value(pgen.enable_equalizer().get(pat));
    chk_noise_gate->value(pgen.enable_noisegate().get(pat));

    assoc_.clear();

    setup_checkbox(chk_compressor, pgen.enable_compressor());
    setup_checkbox(chk_filter, pgen.enable_filter());
    setup_checkbox(chk_pitch, pgen.enable_pitch());
    setup_checkbox(chk_chorus, pgen.enable_modulator());
    setup_checkbox(chk_delay, pgen.enable_delay());
    setup_checkbox(chk_reverb, pgen.enable_reverb());
    setup_checkbox(chk_equalizer, pgen.enable_equalizer());
    setup_checkbox(chk_noise_gate, pgen.enable_noisegate());

    setup_choice(cb_filter, pgen.type_filter());
    setup_choice(cb_pitch, pgen.type_pitch());
    setup_choice(cb_chorus, pgen.type_modulation());
    setup_choice(cb_delay, pgen.type_delay());
    setup_choice(cb_reverb, pgen.type_reverb());

    std::array<Fl_Group *, 6> box_cpr
        {{ box_cpr1, box_cpr2, box_cpr3, box_cpr4, box_cpr5, box_cpr6  }};
    setup_boxes(pgen.enable_compressor().get(pat), pgen.compressor, box_cpr.data(), box_cpr.size());

    // TODO
    

    for (const auto &a : assoc_)
        a->update_value(pat);
}

void Main_Component::setup_checkbox(Fl_Check_Button *chk, PA_Boolean &p)
{
    std::unique_ptr<Association> a(new Association);
    a->access = &p;
    a->value_widget = chk;
    a->kind = Assoc_Check;
    assoc_.push_back(std::move(a));
}

void Main_Component::setup_choice(Fl_Choice *cb, PA_Choice &p)
{
    std::unique_ptr<Association> a(new Association);

    cb->clear();
    for (const std::string &value : p.values)
        cb->add(value.c_str(), 0, nullptr);

    a->access = &p;
    a->value_widget = cb;
    a->kind = Assoc_Choice;
    assoc_.push_back(std::move(a));
}

void Main_Component::setup_boxes(bool enable, const Parameter_Collection &pc, Fl_Group *boxes[], unsigned nboxes)
{
    for (unsigned i = 0; i < nboxes; ++i) {
        Fl_Group *box = boxes[i];
        box->clear();
        box->labeltype(FL_NO_LABEL);
    }
    if (enable) {
        for (size_t i = 0; i < 5; ++i) {
            std::unique_ptr<Association> a(new Association);
            a->flags = Assoc_Name_On_Box|Assoc_Value_On_Label;

            Parameter_Access *pa = pc.slots[i].get();
            a->access = pa;

            Fl_Group *box = boxes[i];
            a->group_box = box;
            int bx = box->x(), by = box->y(), bw = box->w(), bh = box->h();

            int padding = 16;
            int wx = bx + padding, wy = by + padding;
            int ww = bw - 2 * padding, wh = bh - 2 * padding;

            box->labeltype(FL_NORMAL_LABEL);
            box->labelsize(9);
            box->align(FL_ALIGN_TOP|FL_ALIGN_INSIDE);

            box->begin();
            Fl_Dial *dial = new Fl_Dial(wx, wy, ww, wh, pa->name);
            a->value_widget = dial;
            a->kind = Assoc_Dial;
            dial->range(pa->min(), pa->max());
            dial->labeltype(FL_NORMAL_LABEL);
            dial->labelsize(9);
            dial->align(FL_ALIGN_BOTTOM);
            box->end();

            assoc_.push_back(std::move(a));
        }
    }
    for (unsigned i = 0; i < nboxes; ++i) {
        Fl_Group *box = boxes[i];
        box->redraw();
    }
}

void Main_Component::update_midi_outs()
{
    Midi_Out &mido = Midi_Out::instance();
    ch_midi_out->clear();
    if (mido.supports_virtual_port())
        ch_midi_out->add(_("Virtual port"));

    std::vector<std::string> out_ports = mido.get_real_ports();
    for (size_t i = 0, n = out_ports.size(); i < n; ++i)
        ch_midi_out->add(out_ports[i].c_str());
}

void Main_Component::on_changed_midi_out()
{
    int value = ch_midi_out->value();
    Midi_Out &mido = Midi_Out::instance();
    if (mido.supports_virtual_port())
        --value;  // virtual port is the first entry

    mido.open_port(value);
}

void Main_Component::on_selected_patch()
{
    unsigned patchno = get_patch_number();
    if (patchno == ~0u)
        return;

    refresh_patch_display();
}

void Main_Component::on_clicked_import()
{
    Fl_Native_File_Chooser f_chooser(Fl_Native_File_Chooser::BROWSE_FILE);
    f_chooser.title(_("Import..."));
    f_chooser.filter(_("Real Major patch\t*.realpatch\n"
                       "Sysex patch\t*.syx"));

    if (f_chooser.show() != 0)
        return;

    const char *filename = f_chooser.filename();
    Patch pat;
    if (!Patch_Loader::load_patch_file(filename, pat)) {
        fl_message_title(_("Error"));
        fl_alert(_("Could not load the patch file."));
        return;
    }

    Patch_Chooser p_chooser(*pbank_);
    unsigned patchno = p_chooser.show(_("Import into..."), _("Select the patch number:"));
    if ((int)patchno == -1)
        return;

    set_nth_patch(patchno, pat);
}

void Main_Component::on_clicked_export()
{
    unsigned patchno = get_patch_number();
    if (patchno == ~0u)
        return;
    const Patch &pat = pbank_->slot[patchno];

    Fl_Native_File_Chooser chooser(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    chooser.title(_("Export..."));
    chooser.filter(_("Real Major patch\t*.realpatch\n"
                     "Sysex patch\t*.syx"));

    if (chooser.show() != 0)
        return;

    std::string filename = chooser.filename();
    std::vector<uint8_t> data;
    switch (chooser.filter_value()) {
    case 0:
        Patch_Writer::save_realmajor_patch(pat, data);
        if (file_name_extension(filename).empty())
            filename += ".realpatch";
        break;
    case 1:
        Patch_Writer::save_sysex_patch(pat, data);
        if (file_name_extension(filename).empty())
            filename += ".syx";
        break;
    }

    if (fl_access(filename.c_str(), 0) == 0) {
        fl_message_title(_("Confirm overwrite"));
        if (fl_choice(_("The file already exists. Replace it?"), _("No"), _("Yes"), nullptr))
            return;
    }

    FILE_u fh(fl_fopen(filename.c_str(), "wb"));
    if (fwrite(data.data(), 1, data.size(), fh.get()) != data.size()) {
        fl_unlink(filename.c_str());
        fl_message_title(_("Error"));
        fl_alert(_("Could not save the patch file."));
        return;
    }
}

void Main_Component::on_clicked_change()
{
    Midi_Out &mido = Midi_Out::instance();

    unsigned patchno = get_patch_number();
    if (patchno == ~0u)
        return;

    uint8_t msg[2] = {0xc0, (uint8_t)patchno};
    mido.send_message(msg, 2);
}

void Main_Component::on_clicked_send()
{
    Midi_Out &mido = Midi_Out::instance();

    unsigned patchno = get_patch_number();
    if (patchno == ~0u)
        return;
    const Patch &pat = pbank_->slot[patchno];

    std::vector<uint8_t> message;
    Patch_Writer::save_sysex_patch(pat, message);
    mido.send_message(message.data(), message.size());
}
