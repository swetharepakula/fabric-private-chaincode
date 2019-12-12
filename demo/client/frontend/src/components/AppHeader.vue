<!--
Copyright IBM Corp. All Rights Reserved.

SPDX-License-Identifier: Apache-2.0
-->

<template>
  <v-app-bar :clipped-left="true" app>
    <v-toolbar-title class="headline text-uppercase">
      <span>Elbonia Communication Commmission</span>
      <span class="font-weight-light"> - Spectrum Auction</span>
    </v-toolbar-title>

    <v-spacer></v-spacer>

    <v-toolbar-items v-if="auction">
      <v-menu offset-y>
        <template v-slot:activator="{ on }">
          <v-btn color="primary" dark v-on="on">
            {{ auction.state }}
          </v-btn>
        </template>
        <v-list>
          <v-list-item
            v-for="s in auctionStates"
            :key="s"
            @click="UPDATE_AUCTION_STATE(s)"
          >
            <v-list-item-title>{{ s }}</v-list-item-title>
          </v-list-item>
        </v-list>
      </v-menu>

      <v-menu offset-y v-if="auction.state === 'clock'">
        <template v-slot:activator="{ on }">
          <v-btn color="secondary" dark v-on="on">
            Round {{ auction.clockRound }}
          </v-btn>
        </template>
        <v-list>
          <v-list-item
            v-for="index in 4"
            :key="index"
            @click="UPDATE_CLOCK_ROUND(index)"
          >
            <v-list-item-title>Round {{ index }}</v-list-item-title>
          </v-list-item>
        </v-list>
      </v-menu>

      <v-btn text>{{ currentTime }}</v-btn>
    </v-toolbar-items>
  </v-app-bar>
</template>

<script>
import { mapState, mapActions } from "vuex";
import moment from "moment";

export default {
  name: "AuctionHeader",
  data: function() {
    return {
      currentTime: null,
      auctionStates: ["clock", "assign", "done", "failed_fsr"]
    };
  },
  computed: {
    ...mapState({
      auction: state => state.auction
    })
  },
  methods: {
    updateCurrentTime() {
      this.currentTime = moment().format("MM/DD/YYYY, h:mm:ss A z");
    },
    ...mapActions("auction", ["UPDATE_AUCTION_STATE", "UPDATE_CLOCK_ROUND"])
  },
  beforeDestroy() {
    clearInterval(this.$options.interval);
  },
  mounted() {
    this.updateCurrentTime();
    this.$options.interval = setInterval(this.updateCurrentTime, 1000);
  }
};
</script>
